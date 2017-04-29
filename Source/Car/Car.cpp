#include "stdafx.h"
#include "Source\Car\Car.hpp"


Car::Car()
	: Car(sf::Vector2f(), sf::Vector2f(), 0.f)
{
}

Car::Car(sf::Vector2f const & position, sf::Vector2f const & direction, float velocity)
	: mPosition(position),
	  mVelocity(velocity),
	  mDirection(mySFML::Simple::normalize(direction))
{
	this->setVertexArray();
}


void Car::render(sf::RenderWindow * renderWindow) const
{
	renderWindow->draw(mVertexArray);
}

void Car::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	//Determine GasBrakeForce- & WheelAngle-Derivatives from Brain
	mGasBrakeForceDerivative = 0.f;
	mSteeringWheelAngleDerivative = 0.f;

	//Determine GasBrakeForce & WheelAngle
	mGasOrBrakeForce += mGasBrakeForceDerivative * time.asSeconds();
	mSteeringWheelAngle += mSteeringWheelAngleDerivative * time.asSeconds();

	//Determine Total Force
	float centripetalForce = mMass / mDistanceBetweenFrontAndBackWheels * mVelocity * mVelocity * mSteeringWheelAngle; //Has sign from angle!
	sf::Vector2f totalForce = mGasOrBrakeForce * mDirection - centripetalForce * mySFML::Create::createOrthogonalVector(mDirection);

	//Cut total force to be smaller than friction force
	float frictionForce = mFrictionCoefficient * mMass * 9.81f;
	if (mySFML::Simple::lengthOf(totalForce) > frictionForce)
	{
		totalForce = mySFML::Simple::normalize(totalForce) * frictionForce;
	}

	//Evolve velocities according to totalForce
	sf::Vector2f acceleration = totalForce / mMass;
	sf::Vector2f newVelocityVector = mDirection * mVelocity + acceleration * time.asSeconds();
	mVelocity = mySFML::Simple::lengthOf(newVelocityVector);
	if (myMath::Simple::abs(mVelocity) > 1.0E-05f) //Saves directions!!!
	{
		mDirection = mySFML::Simple::normalize(newVelocityVector);
	}

	//Evolve position according to direction & velocity
	mPosition += mDirection * mVelocity * time.asSeconds();

	//Reset intern variables
	this->setVertexArray();
}



void Car::setVertexArray()
{
	sf::Vector2f const carSize(3.f, 1.5f);
	sf::Color const carColor(sf::Color::Red);
	sf::Vector2f upVector = (carSize.x / 2.f) * mDirection;
	sf::Vector2f leftVector = (carSize.y / 2.f) * mySFML::Create::createOrthogonalVector(mDirection);
	mVertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
	mVertexArray.clear();
	mVertexArray.append(sf::Vertex(mPosition + upVector + leftVector, carColor));
	mVertexArray.append(sf::Vertex(mPosition - upVector + leftVector, carColor));
	mVertexArray.append(sf::Vertex(mPosition - upVector - leftVector, carColor));
	mVertexArray.append(sf::Vertex(mPosition + upVector - leftVector, carColor));
}
