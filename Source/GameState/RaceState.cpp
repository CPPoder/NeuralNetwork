#include "stdafx.h"
#include "Source\GameState\RaceState.hpp"

namespace GameState
{
	RaceState::~RaceState()
	{

	}

	void RaceState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		mRaceSimulation.update(frametime, renderWindow);
	}

	void RaceState::render(sf::RenderWindow* renderWindow)
	{
		mRaceSimulation.render(renderWindow);
	}

	Change RaceState::pollGameStateChange()
	{
		RaceSimulation::Event raceEvent = mRaceSimulation.pollRaceSimulationEvent();
		switch (raceEvent)
		{
		case RaceSimulation::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case RaceSimulation::Event::OPEN_RACE_SIMULATION_MENU:
			return Change(Change::Type::PUSH, Change::State::RACE_MENU);
		}
	}

	void RaceState::reactOnESC()
	{
		mRaceSimulation.reactOnESC();
	}

}

