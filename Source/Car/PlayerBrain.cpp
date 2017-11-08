#include "stdafx.h"
#include "Source\Car\PlayerBrain.hpp"

#include "Source\Car\Car.hpp"


BrainType PlayerBrain::getBrainType() const
{
	return BrainType::PLAYER;
}

BrainOutput PlayerBrain::calculateBrainOutput(RaceSimulation const * raceSimPointer, Car const * carPointer)
{
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

