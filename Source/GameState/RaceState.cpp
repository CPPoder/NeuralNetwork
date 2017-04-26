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

	GameStateChange RaceState::pollGameStateChange()
	{
		if (!mCloseRaceState)
		{
			return GameStateChange::NO_CHANGE;
		}
		else
		{
			return GameStateChange::POP;
		}
	}

	void RaceState::reactOnESC()
	{
		mCloseRaceState = true;
	}

}

