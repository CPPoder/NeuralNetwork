#include "stdafx.h"
#include "Source\GameState\MainMenuState.hpp"

namespace GameState
{
	MainMenuState::~MainMenuState()
	{

	}

	void MainMenuState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		
	}

	void MainMenuState::render(sf::RenderWindow* renderWindow)
	{
		
	}

	GameStateChange MainMenuState::pollGameStateChange()
	{
		if (!mCloseMainMenu)
		{
			return GameStateChange::NO_CHANGE;
		}
		else
		{
			return GameStateChange::POP;
		}
	}

	void MainMenuState::reactOnESC()
	{
		mCloseMainMenu = true;
	}

}

