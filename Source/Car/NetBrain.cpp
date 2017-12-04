#include "stdafx.h"
#include "Source\Car\NetBrain.hpp"


#include "Source\Math\myUsefulMath.hpp"

#include "Source\Car\Car.hpp"
#include "Source\MainModules\World.hpp"



NetBrain::NetBrain()
	: mSequentialNet(10u)
{

}



BrainType NetBrain::getBrainType() const
{
	return BrainType::NET;
}

BrainOutput NetBrain::calculateBrainOutput(World const * worldPointer, Car const * carPointer)
{
	//Construct input
	//Distances to borders in varying dense angles:
	Track const & track = worldPointer->getTrackReference();
	for (auto angle : sFullVecOfAngles)
	{
		sf::Vector2f direction = mySFML::Create::createRotatedVector(carPointer->getDirection(), angle);
		direction = 1000.f * mySFML::Simple::normalize(direction);
		Line intersectionLine(carPointer->getPosition(), carPointer->getPosition() + direction);
		std::list<sf::Vector2f> intersections = track.getIntersectionsWithBorder(intersectionLine);
		//Find shortest dist between carPos and intersections
		//Use that as some brain input and save in some member, so that it can be render later!
	}
	
	

	
	//Forward normal vector
	//Actual state(velocity, damage, actualGasBrake, actualSteering)






	float gasBrakeQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;
	float steeringWheelQuotient = static_cast<float>(myMath::Rand::randIntervali(-100, 100)) / 100.f;

	return BrainOutput(gasBrakeQuotient, steeringWheelQuotient);
}





const std::vector<float> NetBrain::sVecOfAngles = { 0.1f, 0.2f, 0.35f, 0.6f, 1.f, 1.5f, 2.5f, 4.f, 8.f, 15.f, 30.f, 45.f, 60.f, 75.f, 90.f, 105.f, 120.f, 135.f, 145.f, 155.f, 165.f, 175.f };

const std::vector<float> NetBrain::sVecOfAngles = []()
{
	std::vector<float> vec;
	vec.push_back(0.f);
	for (auto angle : sVecOfAngles)
	{
		vec.push_back(angle);
	}
	for (auto angle : sVecOfAngles)
	{
		vec.push_back(-angle);
	}
	vec.push_back(180.f);
	return vec;
}();

