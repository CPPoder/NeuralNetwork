#include "stdafx.h"
#include "Source\Car\RandomBrain.hpp"

#include "Source\Math\myUsefulMath.hpp"

#include "Source\Car\Car.hpp"

BrainType RandomBrain::getBrainType() const
{
	return BrainType::RANDOM;
}

BrainOutput RandomBrain::calculateBrainOutput(RaceSimulation const * raceSimPointer, Car const * carPointer)
{
	float gasBrakeQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;
	float steeringWheelQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;

	return BrainOutput(gasBrakeQuotient, steeringWheelQuotient);
}

