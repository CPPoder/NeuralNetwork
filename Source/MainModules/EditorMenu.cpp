#include "stdafx.h"
#include "Source\MainModules\EditorMenu.hpp"


EditorMenu::EditorMenu()
	: mContinueButton(sf::Vector2f(200.f, 200.f), sf::Vector2f(200.f, 50.f), "Continue", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mExitButton(sf::Vector2f(200.f, 300.f), sf::Vector2f(200.f, 50.f), "Exit", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mTitleText(*mFonts.getFont(mySFML::Class::FontName::ARIAL), sf::Vector2f(200.f, 100.f), "Editor Menu", 28u, sf::Color::White)
{

}

EditorMenu::~EditorMenu()
{

}


void EditorMenu::render(sf::RenderWindow * renderWindow)
{
	renderWindow->draw(*mTitleText.pointer);
	mContinueButton.render(renderWindow);
	mExitButton.render(renderWindow);
}

void EditorMenu::update(sf::Time const & time, sf::RenderWindow * renderWindow)
{
	mContinueButton.updateState(renderWindow);
	mExitButton.updateState(renderWindow);

	if (mContinueButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::CONTINUE;
	}
	if (mExitButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::EXIT_EDITOR;
	}
}


EditorMenu::Event EditorMenu::pollEditorMenuEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void EditorMenu::reactOnESC()
{
	mEvent = Event::CONTINUE;
}

