#include "stdafx.h"
#include "Source\Car\Car.hpp"

#include "Source\MainModules\World.hpp"
#include "Source\Math\myUsefulMath.hpp"
#include "Source\Track\Track.hpp"


Car::Car()
	: Car(sf::Vector2f(), sf::Vector2f(), 0.f, BrainType::RANDOM)
{
}

Car::Car(sf::Vector2f const & position, sf::Vector2f const & direction, float velocity, BrainType brainType)
	: mPosition(position),
	  mVelocity(velocity),
	  mDirection(mySFML::Simple::normalize(direction)),
	  pBrain(Brain::constructBrain(brainType)),
	  mVelocityText(*mFonts.getFont(mySFML::Class::FontName::ARIAL), mPosition, "", 12u, sf::Color::Red)
{
	this->setVertexArray();
}

Car::~Car()
{
	delete pBrain;
	pBrain = nullptr;
}

Car::Car(Car const & car)
	: Car(car.mPosition, car.mDirection, car.mVelocity, car.pBrain->getBrainType())
{
}


void Car::render(sf::RenderWindow * renderWindow) const
{
	renderWindow->draw(mVertexArray);
	renderWindow->draw(mTiresVertexArray);
	renderWindow->draw(*mVelocityText.pointer);
}

void Car::update(sf::Time const & time, sf::RenderWindow const * renderWindow, World const * worldPointer)
{
	//Save the day for long times
	sf::Time saveTime = ( (time > sf::seconds(1.f)) ? sf::seconds(1.f) : saveTime = time );

	//Save current State
	sf::Vector2f positionBackup = mPosition;
	sf::Vector2f directionBackup = mDirection;
	float velocityBackup = mVelocity;

	//Let the brain calculate the brain output
	BrainOutput brainOutput = pBrain->calculateBrainOutput(worldPointer, this);
	
	//Update factors using brainOutput
	mGasOrBrakeFactor += brainOutput.gasBrakeFactorDerivative * saveTime.asSeconds();
	mSteeringWheelFactor += brainOutput.steeringWheelFactorDerivative * saveTime.asSeconds();
	mGasOrBrakeFactor = myMath::Simple::trim(-1.f, mGasOrBrakeFactor, 1.f);
	mSteeringWheelFactor = myMath::Simple::trim(-1.f, mSteeringWheelFactor, 1.f);

	//Determine GasForce & BrakeForce (If v>0: Up is gas, down is brake; If v<0: The other way arround!)
	float cuttedV = ((mVelocity >= 0) ? myMath::Simple::max(mVelocity, 0.0001f) : myMath::Simple::min(mVelocity, -0.0001f));
	float absoluteGasForce = myMath::Simple::abs(mGasOrBrakeFactor * mMaximalPower / cuttedV * (1.f - mDamage)); //W=F*s => P=F*v => F=P/v
	float absoluteBrakeForce = myMath::Simple::abs(mGasOrBrakeFactor * mMaximalBrakeForce * (1.f - mDamage));
	float gasOrBrakeForce;
	if (mGasOrBrakeFactor > 0.f)
	{
		//Accelerate, either with gas or brake
		if (mVelocity >= 0.f)
		{
			gasOrBrakeForce = absoluteGasForce;
		}
		else
		{
			gasOrBrakeForce = absoluteBrakeForce;
		}
	}
	else
	{
		//De-Accelerate, either with gas or brake
		if (mVelocity >= 0.f)
		{
			gasOrBrakeForce = -absoluteBrakeForce;
		}
		else
		{
			gasOrBrakeForce = -absoluteGasForce;
		}
	}

	//Determine Rolling Friction
	float rollingFrictionValue = mMass * mRollingFrictionCoefficient * 9.81f;
	sf::Vector2f rollingFriction = rollingFrictionValue * ((mVelocity > 0.f) ? -1.f : 1.f) * mDirection;

	//Determine Total Force
	float centripetalForce = mMass / mDistanceBetweenFrontAndBackWheels * mVelocity * mVelocity * mSteeringWheelFactor * mMaximalSteeringWheelAngle; //Has sign from angle!
	sf::Vector2f totalForce = gasOrBrakeForce * mDirection + centripetalForce * mySFML::Create::createOrthogonalVector(mDirection) + rollingFriction;

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

	//Add Air Friction Force to total force
	float constexpr densityOfAir = 1.3f;
	float airFrictionForceValue = 0.5f * densityOfAir * mAirArea * mCW * mVelocity * mVelocity;
	sf::Vector2f airFrictionForce = ((mVelocity > 0.f) ? -1.f : 1.f) * airFrictionForceValue * mDirection;
	totalForce += airFrictionForce;

	//Evolve velocities according to totalForce
	sf::Vector2f acceleration = totalForce / mMass;
	sf::Vector2f newVelocityVector = mDirection * mVelocity + acceleration * saveTime.asSeconds();
	bool forwards = (mySFML::Simple::scalarProduct(newVelocityVector, mDirection) > 0.f);
	float forwardFactor = (forwards ? 1.f : -1.f);
	mVelocity = mySFML::Simple::lengthOf(newVelocityVector) * forwardFactor;
	if (myMath::Simple::abs(mVelocity) > 1.0E-05f) //Saves directions!!!
	{
		mDirection = forwardFactor * mySFML::Simple::normalize(newVelocityVector);
	}

	//Evolve position according to direction & velocity
	mPosition += mDirection * mVelocity * saveTime.asSeconds();

	//Reset intern variables
	this->setVertexArray();

	//Check for collision and handle
	if (this->checkForBoundaryCollision(worldPointer))
	{
		//Handle damage
		float constexpr damageConstant = 0.0005f;
		mDamage += mVelocity * mVelocity * damageConstant;
		mDamage = myMath::Simple::trim(0.f, mDamage, 1.f);

		//Restore old state
		mPosition = positionBackup;
		mDirection = directionBackup;
		mVelocity = 0.f;

		this->setVertexArray();
	}

	//Debug
	//std::cout << "Pos: " << mPosition.x << " " << mPosition.y << std::endl;
	//std::cout << "Damage: " << mDamage << std::endl;
	//std::cout << "Velocity: " << mVelocity << std::endl;
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
	float tiresAngle = carAngle - mSteeringWheelFactor * mMaximalSteeringWheelAngle; //- due to mSteeringWheelAngle's definition
	sf::Vector2f tiresVector = mySFML::Create::createNormalVectorWithAngle(-tiresAngle) * tiresLength / 2.f; //CreateNormalVector does it in mathematical coordinates!!!
	mTiresVertexArray.setPrimitiveType(sf::PrimitiveType::Lines);
	mTiresVertexArray.clear();
	mTiresVertexArray.append(sf::Vertex(leftTirePos - tiresVector, tiresColor));
	mTiresVertexArray.append(sf::Vertex(leftTirePos + tiresVector, tiresColor));
	mTiresVertexArray.append(sf::Vertex(rightTirePos - tiresVector, tiresColor));
	mTiresVertexArray.append(sf::Vertex(rightTirePos + tiresVector, tiresColor));

	//Set Velocity Text
	mVelocityText.setString(std::to_string(static_cast<int>(mVelocity * 3.6f)));
	mVelocityText.pointer->setPosition(mPosition + sf::Vector2f(1.f, 1.f));
	mVelocityText.pointer->setScale(0.4f, 0.4f);
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
float Car::getGasOrBrakeFactor() const
{
	return mGasOrBrakeFactor;
}
float Car::getSteeringWheelFactor() const
{
	return mSteeringWheelFactor;
}
float Car::getMaximalPower() const
{
	return mMaximalPower;
}
float Car::getMaximalBrakeForce() const
{
	return mMaximalBrakeForce;
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
float Car::getDamage() const
{
	return mDamage;
}
sf::VertexArray const & Car::getVertexArrayReference() const
{
	return mVertexArray;
}


////////
//Setter

void Car::setPosition(sf::Vector2f const & position)
{
	mPosition = position;
	this->setVertexArray();
}


///////////////////////////
//CheckForBoundaryCollision

bool Car::checkForBoundaryCollision(World const * worldPointer) const
{
	Track const & trackReference = worldPointer->getTrackReference();
	return trackReference.checkCollisionWith(*this);
}
