#include "stdafx.h"
#include "Source\MainModules\Editor.hpp"


Editor::Editor()
{

}

Editor::~Editor()
{

}


void Editor::render(sf::RenderWindow * renderWindow)
{
	
}

void Editor::update(sf::Time const & time, sf::RenderWindow * renderWindow)
{
	
}


Editor::Event Editor::pollEditorEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void Editor::reactOnESC()
{
	mEvent = Event::OPEN_EDITOR_MENU;
}

