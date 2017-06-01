#include "stdafx.h"
#include "Source\GameState\EditorState.hpp"

namespace GameState
{
	EditorState::EditorState()
		: mEditor(mEditorWindowSize)
	{

	}

	EditorState::~EditorState()
	{

	}

	void EditorState::update(sf::Time const & frametime, sf::RenderWindow* renderWindow)
	{
		mEditor.update(frametime, renderWindow);
	}

	void EditorState::render(sf::RenderWindow* renderWindow)
	{
		mEditor.render(renderWindow);
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
		if (mWindowAdjusted)
		{
			return std::deque<WindowChange>();
		}
		else
		{
			mWindowAdjusted = true;
			return std::deque<WindowChange>({
				WindowChange(WindowChange::Type::CHANGE_WINDOW_SIZE, WindowChange::Info(mEditorWindowSize)),
				WindowChange(WindowChange::Type::CHANGE_WINDOW_POSITION, WindowChange::Info(sf::Vector2u(), false, sf::Vector2i(250, 100)))
			});
		}
	}

}

