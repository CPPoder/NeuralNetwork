#include "stdafx.h"
#include "Source\MainModules\RaceSimulation.hpp"

#include "Source\Math\myUsefulMath.hpp"

#include "Source\Framework\EventManager.hpp"


RaceSimulation::RaceSimulation()
//	: mTrack(Track::constructCircleTrack(sf::Vector2f(50.f, 50.f), 40.f, 50u, 6.f))
	: mTrack(Track::createRandomTrack())
{
	for (unsigned int i = 0; i < 1; ++i)
	{
		mListOfCars.push_back(Car(mTrack.calculatePositionInTrackNear(sf::Vector2f(10.f, 50.f)), sf::Vector2f(0.f, 1.f), 0.f, BrainType::PLAYER));
	}
}

RaceSimulation::~RaceSimulation()
{

}


void RaceSimulation::render(sf::RenderWindow * renderWindow)
{
	sf::View raceView(sf::FloatRect(0.f, 0.f, 100.f, 100.f));
	sf::View originalView(renderWindow->getView());
	renderWindow->setView(raceView);

	mTrack.render(renderWindow);
	for (auto & car : mListOfCars)
	{
		car.render(renderWindow);
	}

	renderWindow->setView(originalView);
}

void RaceSimulation::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	for (auto & car : mListOfCars)
	{
		car.update(time, renderWindow, this);
	}
	if (EventManager::checkForEvent(EventManager::EventType::KEY_RELEASED))
	{
		EventManager::KeyInfo keyInfo = EventManager::getReleasedKeyInfo();
		if (keyInfo.key == sf::Keyboard::Key::Tab)
		{
			mTrack = Track(Track::createRandomTrack());
			for (auto & car : mListOfCars)
			{
				car.setPosition(mTrack.calculatePositionInTrackNear(car.getPosition()));
			}
		}
	}
}




RaceSimulation::Event RaceSimulation::pollRaceSimulationEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void RaceSimulation::reactOnESC()
{
	mEvent = Event::OPEN_RACE_SIMULATION_MENU;
}





Track const & RaceSimulation::getTrackReference() const
{
	return mTrack;
}

