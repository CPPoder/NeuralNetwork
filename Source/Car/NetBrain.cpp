#include "stdafx.h"
#include "Source\Car\NetBrain.hpp"


#include "Source\Math\myUsefulMath.hpp"

#include "Source\Car\Car.hpp"
#include "Source\MainModules\World.hpp"



NetBrain::NetBrain()
	: mSequentialNet(NetBrain::sNetInputSize)
{
	//Add dense layers here!
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
	unsigned int inputSize = mListOfSeeingLines.size() + 1 + 4; //borderCollisions + forwardProjection + (velocity, damage, actual(GasBrake/Steering))


	float gasBrakeQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;
	float steeringWheelQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;

	//return BrainOutput(gasBrakeQuotient, steeringWheelQuotient);



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

	return BrainOutput(gasBrakeCoefficient * gasBrakeDiff, steeringWheelCoefficient * steeringWheelDiff);


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





const std::array<float, NetBrain::sNumOfAngles> NetBrain::sArrayOfAngles = { 0.3f, 0.6f, 1.2f, 2.5f, 5.f, 12.f, 25.f, 35.f, 50.f, 70.f, 90.f, 110.f, 130.f, 150.f, 165.f, 175.f };

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

