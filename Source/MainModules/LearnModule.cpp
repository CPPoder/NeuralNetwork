#include "stdafx.h"
#include "Source\MainModules\LearnModule.hpp"

LearnModule::LearnModule()
{
	//Create Car with some Neural network brain
	//Create World with this car
}

LearnModule::~LearnModule()
{
}


void LearnModule::render(sf::RenderWindow * renderWindow)
{
	if (pWorld != nullptr)
	{
		pWorld->render(renderWindow);
		pWorld->getCarsReference().front().renderSeeingLinesIfNetBrain(renderWindow, pWorld->getUsedView());
	}
}

void LearnModule::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	if (pWorld == nullptr)
	{
		Track track("./Data/Tracks/test.tr");
		Car neuralNetCar(track.calculatePositionInTrackNear(sf::Vector2f()), sf::Vector2f(1.0f, 0.f), 0.f, BrainType::NET);
		pWorld = new World(track, { neuralNetCar });
	}
	
	sf::Clock clock;
	int counter = 0;
	while (clock.getElapsedTime().asSeconds() < 1.f / 50.f) //50FPS for rendering
	{
		pWorld->update(sf::seconds(1.f / 50.f), renderWindow); //Update jumps of 1/50 seconds
		++counter;
	}
	std::cout << counter << std::endl;
}




LearnModule::Event LearnModule::pollLearnModuleEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void LearnModule::reactOnESC()
{
	mEvent = Event::OPEN_LEARN_MODULE_MENU;
}
