#include "stdafx.h"
#include "Source\MainModules\RaceSimulation.hpp"

#include "Source\Math\myUsefulMath.hpp"


RaceSimulation::RaceSimulation()
	: mTrack(Track::createRandomTrack())
{
	for (unsigned int i = 0; i < 1; ++i)
	{
		mListOfCars.push_back(Car(sf::Vector2f(10.f, 50.f), sf::Vector2f(0.f, 1.f), 3.f, BrainType::PLAYER));
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




RaceSimulation::Event RaceSimulation::pollRaceSimulationEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void RaceSimulation::reactOnESC()
{
	mEvent = Event::OPEN_RACE_SIMULATION_MENU;
}





Track const & RaceSimulation::getTrackReference() const
{
	return mTrack;
}

