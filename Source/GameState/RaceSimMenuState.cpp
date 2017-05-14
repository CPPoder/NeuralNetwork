#include "stdafx.h"
#include "Source\GameState\RaceSimMenuState.hpp"

namespace GameState
{
	RaceSimMenuState::RaceSimMenuState()
	{

	}

	RaceSimMenuState::~RaceSimMenuState()
	{

	}

	void RaceSimMenuState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		mRaceSimMenu.update(frametime, renderWindow);
	}

	void RaceSimMenuState::render(sf::RenderWindow* renderWindow)
	{
		mRaceSimMenu.render(renderWindow);
	}

	Change RaceSimMenuState::pollGameStateChange()
	{
		RaceSimMenu::Event raceSimMenuEvent = mRaceSimMenu.pollRaceSimMenuEvent();
		switch (raceSimMenuEvent)
		{
		case RaceSimMenu::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case RaceSimMenu::Event::EXIT_RACE_SIMULATION:
			return Change(Change::Type::POP_TWICE_AND_PUSH, Change::State::MAIN_MENU);
		case RaceSimMenu::Event::CONTINUE:
			return Change(Change::Type::POP);
		}
	}

	void RaceSimMenuState::reactOnESC()
	{
		mRaceSimMenu.reactOnESC();
	}

	std::deque<WindowChange> RaceSimMenuState::getWindowChanges()
	{
		return std::deque<WindowChange>();
	}

}

