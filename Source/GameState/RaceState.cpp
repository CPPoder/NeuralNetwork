#include "stdafx.h"
#include "Source\GameState\RaceState.hpp"
#include "Source\Framework\Framework.hpp"

namespace GameState
{
	RaceState::RaceState()
		: mRaceSimulation()
	{
		//Framework::setFramerateLimit(true, 20);
	}

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

	std::deque<WindowChange> RaceState::getWindowChanges()
	{
		sf::Vector2u windowSize(1600u, 800u);
		if (mWindowAdjusted)
		{
			return std::deque<WindowChange>();
		}
		else
		{
			mWindowAdjusted = true;
			return std::deque<WindowChange>({ 
				WindowChange(WindowChange::Type::CHANGE_WINDOW_SIZE, WindowChange::Info(windowSize)),
				WindowChange(WindowChange::Type::CHANGE_WINDOW_POSITION, WindowChange::Info(sf::Vector2u(), false, sf::Vector2i(50, 50)))
			});
		}
	}

}

