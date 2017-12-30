#include "stdafx.h"
#include "Source\GameState\LearnState.hpp"
#include "Source\Framework\Framework.hpp"



namespace GameState
{
	LearnState::LearnState()
	{
		//Framework::setFramerateLimit(true, 50);
	}

	LearnState::~LearnState()
	{

	}

	void LearnState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		mLearnModule.update(frametime, renderWindow);
	}

	void LearnState::render(sf::RenderWindow* renderWindow)
	{
		mLearnModule.render(renderWindow);
	}

	Change LearnState::pollGameStateChange()
	{
		LearnModule::Event learnEvent = mLearnModule.pollLearnModuleEvent();
		switch (learnEvent)
		{
		case LearnModule::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case LearnModule::Event::OPEN_LEARN_MODULE_MENU:
			return Change(Change::Type::PUSH, Change::State::RACE_MENU);
		}
	}

	void LearnState::reactOnESC()
	{
		mLearnModule.reactOnESC();
	}

	std::deque<WindowChange> LearnState::getWindowChanges()
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

