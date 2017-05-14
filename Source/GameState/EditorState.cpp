#include "stdafx.h"
#include "Source\GameState\EditorState.hpp"

namespace GameState
{
	EditorState::~EditorState()
	{

	}

	void EditorState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		
	}

	void EditorState::render(sf::RenderWindow* renderWindow)
	{
		
	}

	Change EditorState::pollGameStateChange()
	{
		Editor::Event editorEvent = mEditor.pollEditorEvent();
		switch (editorEvent)
		{
		case Editor::Event::NONE:
			return Change(Change::Type::NO_CHANGE);
		case Editor::Event::OPEN_EDITOR_MENU:
			return Change(Change::Type::PUSH, Change::State::EDITOR_MENU);
		}
	}

	void EditorState::reactOnESC()
	{
		mEditor.reactOnESC();
	}

	std::deque<WindowChange> EditorState::getWindowChanges()
	{
		sf::Vector2u windowSize(800u, 800u);
		if (mWindowAdjusted)
		{
			return std::deque<WindowChange>();
		}
		else
		{
			return std::deque<WindowChange>({ WindowChange(WindowChange::Type::CHANGE_WINDOW_SIZE, WindowChange::Info(windowSize)) });
		}
	}

}

