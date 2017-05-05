#include "stdafx.h"
#include "Source\MainModules\MainMenu.hpp"


MainMenu::MainMenu()
	: mStartSimulationButton(sf::Vector2f(200.f, 200.f), sf::Vector2f(200.f, 50.f), "Start Simulation", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mExitButton(sf::Vector2f(200.f, 400.f), sf::Vector2f(200.f, 50.f), "Exit", mySFML::Class::FontName::ARIAL, 2.f, 18u)
{

}

MainMenu::~MainMenu()
{

}


void MainMenu::render(sf::RenderWindow * renderWindow)
{
	mStartSimulationButton.render(renderWindow);
	mExitButton.render(renderWindow);
}

void MainMenu::update(sf::Time const & time, sf::RenderWindow * renderWindow)
{
	mStartSimulationButton.updateState(renderWindow);
	mExitButton.updateState(renderWindow);

	if (mStartSimulationButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::START_SIMULATION;
	}
	if (mExitButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::EXIT;
	}
}


MainMenu::Event MainMenu::getMainMenuEvent() const
{
	return mEvent;
}

void MainMenu::reactOnESC()
{
	mEvent = Event::EXIT;
}

