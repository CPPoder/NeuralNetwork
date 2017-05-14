#include "stdafx.h"
#include "Source\MainModules\RaceSimulation.hpp"

#include "Source\Math\myUsefulMath.hpp"

#include "Source\Framework\EventManager.hpp"


RaceSimulation::RaceSimulation(sf::Vector2f const & size)
//	: mTrack(Track::constructCircleTrack(sf::Vector2f(50.f, 50.f), 40.f, 50u, 6.f))
//	: mTrack(Track::createRandomTrack())
	: mTrack("./Data/Tracks/test.tr"),
	  mSize(size)
{
	for (unsigned int i = 0; i < 1; ++i)
	{
		mListOfCars.push_back(Car(mTrack.calculatePositionInTrackNear(sf::Vector2f(10.f, 50.f)), sf::Vector2f(0.f, 1.f), 0.f, BrainType::PLAYER));
	}
}

RaceSimulation::~RaceSimulation()
{
	mTrack.saveToFile("./Data/Tracks/test.tr");
}


void RaceSimulation::render(sf::RenderWindow * renderWindow)
{
	sf::View raceView(sf::FloatRect(0.f, 0.f, 400.f, 200.f));
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
			std::cout << "RandomTrackCreation! Type in Number of Deformations: ";
			unsigned int numberOfDeformations;
			std::cin >> numberOfDeformations;

			std::cout << "RandomTrackCreation! Type in Deformation Length: ";
			float deformationLength;
			std::cin >> deformationLength;

			mTrack.deformRandomly(numberOfDeformations, mSize, deformationLength);
			for (auto & car : mListOfCars)
			{
				car.setPosition(mTrack.calculatePositionInTrackNear(car.getPosition()));
			}
		}
		else if (keyInfo.key == sf::Keyboard::Key::D)
		{
			mTrack.doubleNumberOfSegments();
		}
		else if (keyInfo.key == sf::Keyboard::Key::C)
		{
			mTrack = Track(Track::constructCircleTrack(sf::Vector2f(100.f, 100.f), 40.f, 50u, 6.f));
		}
		else if (keyInfo.key == sf::Keyboard::Key::V)
		{
			bool validity = mTrack.checkIfTrackIsValid(mSize);
			if (validity)
			{
				std::cout << "Actual track is valid!" << std::endl;
			}
			else
			{
				std::cout << "Actual track is not valid!" << std::endl;
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

