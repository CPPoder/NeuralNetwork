#include "stdafx.h"
#include "Source\Simulation\RaceSimulation.hpp"


RaceSimulation::RaceSimulation()
	: mTrack(this->constructCircleTrack())
{

}

RaceSimulation::~RaceSimulation()
{

}


void RaceSimulation::render(sf::RenderWindow * renderWindow)
{
	mTrack.render(renderWindow);
}

void RaceSimulation::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{

}



std::list<std::pair<sf::Vector2f, float>> RaceSimulation::constructCircleTrack()
{
	float radius(200.f);
	sf::Vector2f center(300.f, 300.f);
	float width(30.f);
	unsigned int numberOfPoints(30u);

	std::list<std::pair<sf::Vector2f, float>> list;
	for (unsigned int i = 0; i < numberOfPoints; ++i)
	{
		list.push_back(std::make_pair(radius * mySFML::Create::createNormalVectorWithAngle(static_cast<float>(i) / (numberOfPoints + 1u) * 2.f * myMath::Const::PIf) + center, width));
	}

	return list;
}
