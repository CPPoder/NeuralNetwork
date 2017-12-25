#include "stdafx.h"
#include "Source\MainModules\RaceSimulation.hpp"

#include "Source\Math\myUsefulMath.hpp"

#include "Source\Framework\EventManager.hpp"


RaceSimulation::RaceSimulation()
{
	Track track("./Data/Tracks/learning.tr");
	std::list<Car> listOfCars;
	sf::Vector2f pos = track.calculatePositionInTrackNear(track.getStartPosition());
	sf::Vector2f dir = track.getForwardDirectionAt(pos);
	listOfCars.push_back(Car(pos, dir, 0.f, BrainType::PLAYER));
	for (unsigned int i = 0; i < 1; ++i)
	{
		NetBrain netBrain;
		netBrain.loadFromFile("./Data/Nets/start.net");
		listOfCars.push_back(Car(pos, dir, 0.f, netBrain));
	}
	mWorld = World(track, listOfCars);
	mWorld.setUsedViewToWantedView();
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



