#include "stdafx.h"
#include "Source\Car\NetBrain.hpp"


#include "Source\Math\myUsefulMath.hpp"

#include "Source\Car\Car.hpp"
#include "Source\MainModules\World.hpp"
#include "Source\NeuralNet\DenseLayer.hpp"



NetBrain::NetBrain()
	: mSequentialNet(NetBrain::sNetInputSize)
{
	//Create net with one hidden layer!
	//This layer shall have between input number and output number of neurons! Choose:
	
	//Add hidden layer
	unsigned int constexpr hiddenLayerSize = (sNetInputSize + sNetOutputSize) / 2u;
	mSequentialNet.addLayer(DenseLayer(hiddenLayerSize, Activation::Atan));

	//Add output layer
	mSequentialNet.addLayer(DenseLayer(sNetOutputSize, Activation::Atan));
	mSequentialNet.compile();

	//Preset somehow good values
	float constexpr alphaGas = 0.03f;
	float constexpr betaGas = 0.2f;
	float constexpr alphaSteer = 0.04f;
	float constexpr betaSteer = 0.9f;
	float constexpr vWanted = 4.f;

	Mat::Matrix<NetNodeType> matrix0(Mat::XY(sNetInputSize, hiddenLayerSize), 0.f);
	matrix0.at(Mat::XY(sNetInputSize - 4u, 0u)) = alphaGas * betaGas;
	matrix0.at(Mat::XY(sNetInputSize - 2u, 0u)) = -betaGas;
	matrix0.at(Mat::XY(sNetInputSize - 1u, 1u)) = -betaSteer;
	matrix0.at(Mat::XY(sNumOfAngles + 2u, 1u)) = -alphaSteer * betaSteer;
	matrix0.at(Mat::XY(2u, 1u)) = alphaSteer * betaSteer;
	mSequentialNet.setMatrixOfLayer(0u, matrix0);

	Mat::Vector<NetNodeType> bias0(hiddenLayerSize, 0.f);
	bias0.at(0) = 0.f - vWanted * alphaGas * betaGas;
	bias0.at(1) = 0.f;
	mSequentialNet.setBiasOfLayer(0u, bias0);

	Mat::Matrix<NetNodeType> matrix1(Mat::XY(hiddenLayerSize, sNetOutputSize), 0.f);
	matrix1.at(Mat::XY(0u, 0u)) = -1.f;
	matrix1.at(Mat::XY(1u, 1u)) = 1.f;
	mSequentialNet.setMatrixOfLayer(1u, matrix1);

	Mat::Vector<NetNodeType> bias1(sNetOutputSize, 0.f);
	bias1.at(0u) = -0.f;
	bias1.at(1u) = -0.f;
	mSequentialNet.setBiasOfLayer(1u, bias1);


	//Compile net
	//mSequentialNet.compile();
}

NetBrain::NetBrain(NetBrain const & netBrain)
	: mSequentialNet(NetBrain::sNetInputSize)
{
	mSequentialNet = netBrain.mSequentialNet;
	mListOfSeeingLines = netBrain.mListOfSeeingLines;
}


NetBrain& NetBrain::operator=(NetBrain const & netBrain)
{
	mSequentialNet = netBrain.mSequentialNet;
	mListOfSeeingLines = netBrain.mListOfSeeingLines;
	return *this;
}



BrainType NetBrain::getBrainType() const
{
	return BrainType::NET;
}

BrainOutput NetBrain::calculateBrainOutput(World const * worldPointer, Car const * carPointer)
{
	////////////////////////////////////////
	//Construct variables used for net input

	//Distances to borders in varying dense angles:
	Track const & track = worldPointer->getTrackReference();
	sf::Vector2f carPos = carPointer->getPosition();
	mListOfSeeingLines.clear();
	for (auto angle : sFullArrayOfAngles)
	{
		sf::Vector2f direction = mySFML::Create::createRotatedVector(carPointer->getDirection(), angle/180.f*myMath::Const::PIf);
		direction = 1000.f * mySFML::Simple::normalize(direction);
		Line intersectionLine(carPos, carPos + direction);
		std::list<sf::Vector2f> intersections = track.getIntersectionsWithBorder(intersectionLine);

		//Find shortest dist between carPos and intersections
		sf::Vector2f shortestDistPoint = *std::min_element(intersections.begin(), intersections.end(), [carPos](sf::Vector2f v1, sf::Vector2f v2){ return (mySFML::Simple::lengthOf(carPos - v1) <= (mySFML::Simple::lengthOf(carPos - v2))); });
		
		//Use mListOfSeeingLines as brain input, so that it can be rendered later!
		mListOfSeeingLines.push_back(Line(carPos, shortestDistPoint));
	}
	
	//Projection of forward normal vector onto carDirection
	//(greater zero means right direction! But no direct information! E.g. no information whether left or right is correct!)
	float forwardProjection = mySFML::Simple::scalarProduct(mySFML::Simple::normalize(carPointer->getDirection()), mySFML::Simple::normalize(track.getForwardDirectionAt(carPos)));
	
	//Actual state(velocity, damage, actualGasBrake, actualSteering)
	float velocity = carPointer->getVelocity();
	float damage = carPointer->getDamage();
	float actualGasBrake = carPointer->getGasOrBrakeFactor();
	float actualSteering = carPointer->getSteeringWheelFactor();


	//////////////////////////////////
	//Use those variables as net input
	NetInput netInput(sNetInputSize);
	std::list<Line>::const_iterator it = mListOfSeeingLines.cbegin();
	for (unsigned int i = 0; i < sFullNumOfAngles; ++i, ++it)
	{
		netInput.at(i) = it->getLength();
	}
	netInput.at(sFullNumOfAngles) = forwardProjection;
	netInput.at(sFullNumOfAngles + 1) = velocity;
	netInput.at(sFullNumOfAngles + 2) = damage;
	netInput.at(sFullNumOfAngles + 3) = actualGasBrake;
	netInput.at(sFullNumOfAngles + 4) = actualSteering;
	NetOutput netOutput = mSequentialNet.apply(netInput);


	

	return BrainOutput(netOutput.at(0), netOutput.at(1));



	bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
	bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
	bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
	bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);

	float acceleration = 0.f;
	float turning = 0.f;

	if (up)
	{
		acceleration += 1.f;
	}
	if (left)
	{
		turning -= 1.f;
	}
	if (down)
	{
		acceleration -= 1.f;
	}
	if (right)
	{
		turning += 1.f;
	}

	float gasBrakeFactor = carPointer->getGasOrBrakeFactor();
	float steeringWheelFactor = carPointer->getSteeringWheelFactor();

	float gasBrakeDiff = acceleration - gasBrakeFactor;
	float steeringWheelDiff = turning - steeringWheelFactor;

	float constexpr gasBrakeCoefficient = 20.0f;
	float constexpr steeringWheelCoefficient = 20.0f;

	//return BrainOutput(gasBrakeCoefficient * gasBrakeDiff, steeringWheelCoefficient * steeringWheelDiff);


}




void NetBrain::mutate()
{
	mSequentialNet.mutate(Mutation());
}

void NetBrain::setEntriesRandom()
{
	mSequentialNet.setEntriesRandom();
}





void NetBrain::renderSeeingLines(sf::RenderWindow* renderWindow, sf::View view) const
{
	sf::VertexArray vertexArray(sf::PrimitiveType::Lines);
	for (auto const & line : mListOfSeeingLines)
	{
		vertexArray.append(sf::Vertex(line.vertex1, sf::Color::Red));
		vertexArray.append(sf::Vertex(line.vertex2, sf::Color::Red));
	}

	sf::View originalView = renderWindow->getView();
	renderWindow->setView(view);
	renderWindow->draw(vertexArray);
	renderWindow->setView(originalView);
}


void NetBrain::saveToFile(std::string const & path) const
{
	mSequentialNet.saveToFile(path);
}

void NetBrain::loadFromFile(std::string const & path)
{
	mSequentialNet.loadFromFile(path);
}



//const std::array<float, NetBrain::sNumOfAngles> NetBrain::sArrayOfAngles = { 0.3f, 0.6f, 1.2f, 2.5f, 5.f, 12.f, 25.f, 35.f, 50.f, 70.f, 90.f, 110.f, 130.f, 150.f, 165.f, 175.f };
//const std::array<float, NetBrain::sNumOfAngles> NetBrain::sArrayOfAngles = { 2.f, 5.f, 10.f, 25.f, 45.f, 90.f, 135.f };
const std::array<float, NetBrain::sNumOfAngles> NetBrain::sArrayOfAngles = { 5.f, 20.f, 45.f, 90.f };

const std::array<float, NetBrain::sFullNumOfAngles> NetBrain::sFullArrayOfAngles = []()
{
	std::array<float, NetBrain::sFullNumOfAngles> arr;
	arr.front() = 0.f;
	unsigned int arrPos = 1u;
	for (auto angle : sArrayOfAngles)
	{
		arr.at(arrPos) = angle;
		++arrPos;
	}
	for (auto angle : sArrayOfAngles)
	{
		arr.at(arrPos) = -angle;
		++arrPos;
	}
	arr.back() = 180.f;
	return arr;
}();

