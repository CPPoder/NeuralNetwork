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
	float maximalGasBrakeForce = carPointer->getMaximalGasBrakeForce();
	float maximalSteeringWheelAngle = carPointer->getMaximalSteeringWheelAngle();
	
	float gasBrakeQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;
	float steeringWheelQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;

	//std::cout << gasBrakeQuotient * 100 << " " << steeringWheelQuotient * 100 << std::endl;
	

	return BrainOutput(maximalGasBrakeForce * gasBrakeQuotient, maximalSteeringWheelAngle * steeringWheelQuotient);
}

