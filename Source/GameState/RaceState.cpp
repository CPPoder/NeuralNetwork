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
		if (!mCloseRaceState)
		{
			return Change(Change::Type::NO_CHANGE);
		}
		else
		{
			return Change(Change::Type::POP);
		}
	}

	void RaceState::reactOnESC()
	{
		mCloseRaceState = true;
	}

}

