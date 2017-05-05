#include "stdafx.h"
#include "Source\MainModules\RaceSimulation.hpp"

#include "Source\Math\myUsefulMath.hpp"


RaceSimulation::RaceSimulation()
	: mTrack(this->constructCircleTrack())
{
	for (unsigned int i = 0; i < 1; ++i)
	{
		mListOfCars.push_back(Car(sf::Vector2f(10.f, 50.f), sf::Vector2f(0.f, 1.f), 3.f));
	}
}

RaceSimulation::~RaceSimulation()
{

}


void RaceSimulation::render(sf::RenderWindow * renderWindow)
{
	sf::View raceView(sf::FloatRect(0.f, 0.f, 100.f, 100.f));
	sf::View originalView(renderWindow->getView());
	renderWindow->setView(raceView);

	mTrack.render(renderWindow);
	for (auto & car : mListOfCars)
	{
		car.render(renderWindow);
	}

	renderWindow->setView(originalView);
}

void RaceSimulation::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	for (auto & car : mListOfCars)
	{
		car.update(time, renderWindow, this);
	}
}


Track const & RaceSimulation::getTrackReference() const
{
	return mTrack;
}





std::list<std::pair<sf::Vector2f, float>> RaceSimulation::constructCircleTrack()
{
	float radius(40.f);
	sf::Vector2f center(50.f, 50.f);
	float width(5.f);
	unsigned int numberOfPoints(30u);

	std::list<std::pair<sf::Vector2f, float>> list;
	for (unsigned int i = 0; i < numberOfPoints; ++i)
	{
		list.push_back(std::make_pair(radius * mySFML::Create::createNormalVectorWithAngle(static_cast<float>(i) / (numberOfPoints + 1u) * 2.f * myMath::Const::PIf) + center, width));
	}

	return list;
}
