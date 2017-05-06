#include "stdafx.h"
#include "Source\GameState\EditorMenuState.hpp"

namespace GameState
{
	EditorMenuState::EditorMenuState()
	{

	}

	EditorMenuState::~EditorMenuState()
	{

	}

	void EditorMenuState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		mEditorMenu.update(frametime, renderWindow);
	}

	void EditorMenuState::render(sf::RenderWindow* renderWindow)
	{
		mEditorMenu.render(renderWindow);
	}

	Change EditorMenuState::pollGameStateChange()
	{
		EditorMenu::Event editorMenuEvent = mEditorMenu.pollEditorMenuEvent();
		switch (editorMenuEvent)
		{
		case EditorMenu::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case EditorMenu::Event::EXIT_EDITOR:
			return Change(Change::Type::POP_TWICE_AND_PUSH, Change::State::MAIN_MENU);
		case EditorMenu::Event::CONTINUE:
			return Change(Change::Type::POP);
		}
	}

	void EditorMenuState::reactOnESC()
	{
		mEditorMenu.reactOnESC();
	}

}

