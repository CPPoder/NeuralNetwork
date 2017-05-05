#include "stdafx.h"
#include "Source\Car\Car.hpp"

#include "Source\Simulation\RaceSimulation.hpp"
#include "Source\Math\myUsefulMath.hpp"


Car::Car()
	: Car(sf::Vector2f(), sf::Vector2f(), 0.f)
{
}

Car::Car(sf::Vector2f const & position, sf::Vector2f const & direction, float velocity)
	: mPosition(position),
	  mVelocity(velocity),
	  mDirection(mySFML::Simple::normalize(direction)),
	  pBrain(Brain::constructBrain(BrainType::PLAYER))
{
	this->setVertexArray();
}

Car::~Car()
{
	delete pBrain;
	pBrain = nullptr;
}

Car::Car(Car const & car)
	: Car(car.mPosition, car.mDirection, car.mVelocity)
{
}


void Car::render(sf::RenderWindow * renderWindow) const
{
	renderWindow->draw(mVertexArray);
	renderWindow->draw(mTiresVertexArray);
}

void Car::update(sf::Time const & time, sf::RenderWindow const * renderWindow, RaceSimulation const * raceSimPointer)
{
	//Save current State
	sf::Vector2f positionBackup = mPosition;
	sf::Vector2f directionBackup = mDirection;
	float velocityBackup = mVelocity;

	//Determine GasBrakeForce- & WheelAngle-Derivatives from Brain
	BrainOutput brainOutput = pBrain->calculateBrainOutput(raceSimPointer, this);
	mGasBrakeForceDerivative = brainOutput.gasBrakeForceDerivative;
	mSteeringWheelAngleDerivative = brainOutput.steeringWheelAngleDerivative;
	
	//Determine GasBrakeForce & WheelAngle
	mGasOrBrakeForce += mGasBrakeForceDerivative * time.asSeconds();
	mSteeringWheelAngle += mSteeringWheelAngleDerivative * time.asSeconds();
	mGasOrBrakeForce = myMath::Simple::trim(-mMaximalGasOrBrakeForce, mGasOrBrakeForce, mMaximalGasOrBrakeForce);
	mSteeringWheelAngle = myMath::Simple::trim(-mMaximalSteeringWheelAngle, mSteeringWheelAngle, mMaximalSteeringWheelAngle);

	//Determine Total Force
	float centripetalForce = mMass / mDistanceBetweenFrontAndBackWheels * mVelocity * mVelocity * mSteeringWheelAngle; //Has sign from angle!
	sf::Vector2f totalForce = mGasOrBrakeForce * mDirection + centripetalForce * mySFML::Create::createOrthogonalVector(mDirection);

	//Cut total force to be smaller than friction force
	float frictionForce = mFrictionCoefficient * mMass * 9.81f;
	if (mySFML::Simple::lengthOf(totalForce) > frictionForce)
	{
		totalForce = mySFML::Simple::normalize(totalForce) * frictionForce;
		mDrifting = true;
	}
	else
	{
		mDrifting = false;
	}

	//Evolve velocities according to totalForce
	sf::Vector2f acceleration = totalForce / mMass;
	sf::Vector2f newVelocityVector = mDirection * mVelocity + acceleration * time.asSeconds();
	bool forwards = (mySFML::Simple::scalarProduct(newVelocityVector, mDirection) > 0.f);
	float forwardFactor = (forwards ? 1.f : -1.f);
	mVelocity = mySFML::Simple::lengthOf(newVelocityVector) * forwardFactor;
	if (myMath::Simple::abs(mVelocity) > 1.0E-05f) //Saves directions!!!
	{
		mDirection = forwardFactor * mySFML::Simple::normalize(newVelocityVector);
	}

	//Evolve position according to direction & velocity
	mPosition += mDirection * mVelocity * time.asSeconds();

	//Reset intern variables
	this->setVertexArray();

	//Check for collision and handle
	if (this->checkForBoundaryCollision(raceSimPointer))
	{
		//Restore old state
		mPosition = positionBackup;
		mDirection = directionBackup;
		mVelocity = 0.f;

		this->setVertexArray();
	}
}



void Car::setVertexArray()
{
	//Car Vertex Array
	sf::Vector2f const carSize(3.f, 1.5f);
	sf::Color carColor = (mDrifting ? sf::Color(255, 100, 100) : sf::Color::Red);
	sf::Vector2f upVector = (carSize.x / 2.f) * mDirection;
	sf::Vector2f leftVector = (carSize.y / 2.f) * mySFML::Create::createOrthogonalVector(mDirection);
	mVertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
	mVertexArray.clear();
	mVertexArray.append(sf::Vertex(mPosition + upVector + leftVector, carColor));
	mVertexArray.append(sf::Vertex(mPosition - upVector + leftVector, carColor));
	mVertexArray.append(sf::Vertex(mPosition - upVector - leftVector, carColor));
	mVertexArray.append(sf::Vertex(mPosition + upVector - leftVector, carColor));

	//Tires Vertex Array
	float const tiresLength(1.0f);
	sf::Color const tiresColor(sf::Color::Red);
	sf::Vector2f upVecToTires = mDistanceBetweenFrontAndBackWheels / 2.f * mySFML::Simple::normalize(upVector);
	sf::Vector2f leftTirePos = mPosition + leftVector + upVecToTires;
	sf::Vector2f rightTirePos = mPosition - leftVector + upVecToTires;
	float carAngle = mySFML::Simple::angleOf(mDirection);
	float tiresAngle = carAngle - mSteeringWheelAngle; //- due to mSteeringWheelAngle's definition
	sf::Vector2f tiresVector = mySFML::Create::createNormalVectorWithAngle(-tiresAngle) * tiresLength / 2.f; //CreateNormalVector does it in mathematical coordinates!!!
	mTiresVertexArray.setPrimitiveType(sf::PrimitiveType::Lines);
	mTiresVertexArray.clear();
	mTiresVertexArray.append(sf::Vertex(leftTirePos - tiresVector, tiresColor));
	mTiresVertexArray.append(sf::Vertex(leftTirePos + tiresVector, tiresColor));
	mTiresVertexArray.append(sf::Vertex(rightTirePos - tiresVector, tiresColor));
	mTiresVertexArray.append(sf::Vertex(rightTirePos + tiresVector, tiresColor));
}



////////
//Getter

sf::Vector2f Car::getPosition() const
{
	return mPosition;
}
sf::Vector2f Car::getDirection() const
{
	return mDirection;
}
float Car::getVelocity() const
{
	return mVelocity;
}
float Car::getGasBrakeForce() const
{
	return mGasOrBrakeForce;
}
float Car::getSteeringWheelAngle() const
{
	return mSteeringWheelAngle;
}
float Car::getMaximalGasBrakeForce() const
{
	return mMaximalGasOrBrakeForce;
}
float Car::getMaximalSteeringWheelAngle() const
{
	return mMaximalSteeringWheelAngle;
}
float Car::getMass() const
{
	return mMass;
}
float Car::getFrictionCoefficient() const
{
	return mFrictionCoefficient;
}
float Car::getDistanceBetweenFrontAndBackWheels() const
{
	return mDistanceBetweenFrontAndBackWheels;
}
sf::VertexArray const & Car::getVertexArrayReference() const
{
	return mVertexArray;
}




///////////////////////////
//CheckForBoundaryCollision

bool Car::checkForBoundaryCollision(RaceSimulation const * raceSimPointer) const
{
	Track const & trackReference = raceSimPointer->getTrackReference();
	return trackReference.checkCollisionWith(*this);
}
