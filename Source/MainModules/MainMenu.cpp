#include "stdafx.h"
#include "Source\MainModules\MainMenu.hpp"


MainMenu::MainMenu()
	: mStartSimulationButton(sf::Vector2f(200.f, 200.f), sf::Vector2f(200.f, 50.f), "Start Simulation", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mExitButton(sf::Vector2f(200.f, 400.f), sf::Vector2f(200.f, 50.f), "Exit", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mStartEditorButton(sf::Vector2f(200.f, 300.f), sf::Vector2f(200.f, 50.f), "Start Track Editor", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mTitleText(*mFonts.getFont(mySFML::Class::FontName::ARIAL), sf::Vector2f(200.f, 100.f), "Neural Networks", 28u, sf::Color::White)
{

}

MainMenu::~MainMenu()
{

}


void MainMenu::render(sf::RenderWindow * renderWindow)
{
	renderWindow->draw(*mTitleText.pointer);
	mStartSimulationButton.render(renderWindow);
	mExitButton.render(renderWindow);
	mStartEditorButton.render(renderWindow);
}

void MainMenu::update(sf::Time const & time, sf::RenderWindow * renderWindow)
{
	mStartSimulationButton.updateState(renderWindow);
	mExitButton.updateState(renderWindow);
	mStartEditorButton.updateState(renderWindow);

	if (mStartSimulationButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::START_SIMULATION;
	}
	if (mExitButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::EXIT;
	}
	if (mStartEditorButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::OPEN_EDITOR;
	}
}


MainMenu::Event MainMenu::pollMainMenuEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void MainMenu::reactOnESC()
{
	mEvent = Event::EXIT;
}

