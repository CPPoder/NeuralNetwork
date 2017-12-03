#include "stdafx.h"
#include "Source\MainModules\RaceSimulation.hpp"

#include "Source\Math\myUsefulMath.hpp"

#include "Source\Framework\EventManager.hpp"


RaceSimulation::RaceSimulation()
{
	Track track("./Data/Tracks/test.tr");
	std::list<Car> listOfCars;
	sf::Vector2f pos = track.calculatePositionInTrackNear(track.getStartPosition());
	sf::Vector2f dir = track.getForwardDirectionAt(pos);
	listOfCars.push_back(Car(pos, dir, 0.f, BrainType::PLAYER));
	for (unsigned int i = 0; i < 0; ++i)
	{
		listOfCars.push_back(Car(track.calculatePositionInTrackNear(track.getStartPosition()), sf::Vector2f(0.f, 1.f), 0.f, BrainType::RANDOM));
	}
	mWorld = World(track, listOfCars);
}

RaceSimulation::~RaceSimulation()
{
}


void RaceSimulation::render(sf::RenderWindow * renderWindow)
{
	mWorld.render(renderWindow);
}

void RaceSimulation::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	mWorld.update(time, renderWindow);
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



