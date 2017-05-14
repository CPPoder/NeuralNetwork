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
		MainMenu::Event mainMenuEvent = mMainMenu.pollMainMenuEvent();
		switch (mainMenuEvent)
		{
		case MainMenu::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case MainMenu::Event::EXIT:
			return Change(Change::Type::POP);
		case MainMenu::Event::START_SIMULATION:
			return Change(Change::Type::REPLACE, Change::State::RACE);
		case MainMenu::Event::OPEN_EDITOR:
			return Change(Change::Type::REPLACE, Change::State::EDITOR);
		}
	}

	void MainMenuState::reactOnESC()
	{
		mMainMenu.reactOnESC();
	}

	std::deque<WindowChange> MainMenuState::getWindowChanges()
	{
		sf::Vector2u windowSize(800u, 800u);
		if (mWindowAdjusted)
		{
			return std::deque<WindowChange>();
		}
		else
		{
			mWindowAdjusted = true;
			return std::deque<WindowChange>({ WindowChange(WindowChange::Type::CHANGE_WINDOW_SIZE, WindowChange::Info(windowSize)) });
		}
	}

}

