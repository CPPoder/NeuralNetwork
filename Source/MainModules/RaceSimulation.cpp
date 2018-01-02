#include "stdafx.h"
#include "Source\MainModules\RaceSimulation.hpp"

#include "Source\Math\myUsefulMath.hpp"

#include "Source\Framework\EventManager.hpp"

#include <chrono>
#include <thread>


RaceSimulation::RaceSimulation()
{
	pWorld = new World(Track("./Data/Tracks/learning.tr"), {});
	Track const & track = pWorld->getTrackReference();

	sf::Vector2f pos = track.getStartPosition();
	sf::Vector2f dir = track.getForwardDirectionAt(pos);
	pWorld->clearCars();
	Car playerCar(pos, dir, 0.f, BrainType::PLAYER);
	pWorld->addCar(playerCar);

	for (unsigned int i = 0; i < 1; ++i)
	{
		NetBrain netBrain = NetBrain();
		netBrain.loadFromFile("./Data/Nets/learn.net");
		Car neuralNetCar(pos, dir, 0.f, netBrain);
		pWorld->addCar(neuralNetCar);
	}

	pWorld->setUsedViewToWantedView();
}

RaceSimulation::~RaceSimulation()
{
	delete pWorld;
	pWorld = nullptr;
}


void RaceSimulation::render(sf::RenderWindow * renderWindow)
{
	pWorld->render(renderWindow);
}

void RaceSimulation::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	//mWorld.update(time, renderWindow);
	pWorld->update(sf::seconds(1.f / 20.f), renderWindow);
	std::this_thread::sleep_for(std::chrono::milliseconds(25));
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



