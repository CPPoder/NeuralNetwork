#include "stdafx.h"
#include "Source\GameState\LearnMenuState.hpp"



namespace GameState
{
	LearnMenuState::LearnMenuState()
	{

	}

	LearnMenuState::~LearnMenuState()
	{

	}

	void LearnMenuState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		mLearnModuleMenu.update(frametime, renderWindow);
	}

	void LearnMenuState::render(sf::RenderWindow* renderWindow)
	{
		mLearnModuleMenu.render(renderWindow);
	}

	Change LearnMenuState::pollGameStateChange()
	{
		LearnModuleMenu::Event learnModuleMenuEvent = mLearnModuleMenu.pollLearnModuleMenuEvent();
		switch (learnModuleMenuEvent)
		{
		case LearnModuleMenu::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case LearnModuleMenu::Event::EXIT_LEARN_MODULE:
			return Change(Change::Type::POP_TWICE_AND_PUSH, Change::State::MAIN_MENU);
		case LearnModuleMenu::Event::CONTINUE:
			return Change(Change::Type::POP);
		}
	}

	void LearnMenuState::reactOnESC()
	{
		mLearnModuleMenu.reactOnESC();
	}

	std::deque<WindowChange> LearnMenuState::getWindowChanges()
	{
		return std::deque<WindowChange>();
	}

}


