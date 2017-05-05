#include "stdafx.h"
#include "Source\GameState\MainMenuState.hpp"

namespace GameState
{
	MainMenuState::MainMenuState()
	{

	}

	MainMenuState::~MainMenuState()
	{

	}

	void MainMenuState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		mMainMenu.update(frametime, renderWindow);
	}

	void MainMenuState::render(sf::RenderWindow* renderWindow)
	{
		mMainMenu.render(renderWindow);
	}

	Change MainMenuState::pollGameStateChange()
	{
		MainMenu::Event mainMenuEvent = mMainMenu.getMainMenuEvent();
		switch (mainMenuEvent)
		{
		case MainMenu::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case MainMenu::Event::EXIT:
			return Change(Change::Type::POP);
		case MainMenu::Event::START_SIMULATION:
			return Change(Change::Type::REPLACE, Change::State::RACE);
		}
	}

	void MainMenuState::reactOnESC()
	{
		mMainMenu.reactOnESC();
	}

}

