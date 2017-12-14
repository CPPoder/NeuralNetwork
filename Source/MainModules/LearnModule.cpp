#include "stdafx.h"
#include "Source\MainModules\LearnModule.hpp"


LearnModule::LearnModule()
{
	Track track("./Data/Tracks/test.tr");
	pWorld = new World(track, {});
}

LearnModule::~LearnModule()
{
	delete pWorld;
	pWorld = nullptr;
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
	//Evolve 20s for each learning sample!
	//Each sample starts with a mutated brain, is accepted for better rates and discarded for worse rates!
	//Rate function: length of covered track!

	//Set constants
	sf::Time const SIMULATION_TIME = sf::seconds(20.f); //This could be made varying!
	sf::Time const INTERRUPT_FOR_GAME_LOOP_TIME = sf::seconds(1.f / 50.f);
	sf::Time const WORLD_UPDATE_TIME = sf::seconds(1.f / 10.f); //This can probably be chosen much bigger: e.g. 1/10. This would increase the learning rate by a factor of 5 and would probably have no impact on the driving behaviour! (Humans change there behaviour almost only every second!)

	//Initialize first sample
	if (mFirstSample && pWorld->getCarsReference().empty())
	{
		sf::Vector2f carPos = pWorld->getTrackReference().getStartPosition();
		mBestNetBrain = NetBrain(); //Load here from some file later!
		mCurrentNetBrain = mBestNetBrain;
		Car neuralNetCar(carPos, pWorld->getTrackReference().getForwardDirectionAt(carPos), 0.f, mCurrentNetBrain);
		pWorld->addCar(neuralNetCar);
		pWorld->setUsedViewToWantedView();
		mSimulatedTimeOfCurrentSample = sf::Time::Zero;
	}

	if (mSimulatedTimeOfCurrentSample > SIMULATION_TIME) //Rate, accept/discard, initialize new mutated sample
	{
		//Reset mSimulatedTimeOfCurrentSample
		mSimulatedTimeOfCurrentSample = sf::Time::Zero;

		//Rate
		float rate = pWorld->getTrackReference().getDistanceFromStartTo(pWorld->getCarsReference().front().getPosition());
		std::cout << "Rate: " << rate << std::endl;
		//Accept, Discard
		if (mFirstSample)
		{
			mFirstSample = false;
			mBestRate = rate;
		}
		else
		{
			if (rate > mBestRate)
			{
				mBestRate = rate;
				mBestNetBrain = mCurrentNetBrain;
			}
			else
			{
				mCurrentNetBrain = mBestNetBrain;
			}
		}

		//Mutate currentNetBrain
		mCurrentNetBrain.mutate();

		//Use new current net brain for new car
		sf::Vector2f carPos = pWorld->getTrackReference().getStartPosition();
		Car neuralNetCar(carPos, pWorld->getTrackReference().getForwardDirectionAt(carPos), 0.f, mCurrentNetBrain);
		pWorld->clearCars();
		pWorld->addCar(neuralNetCar);

	}
	else //Simulate
	{
		sf::Clock interruptForGameLoopClock;
		while (interruptForGameLoopClock.getElapsedTime() < INTERRUPT_FOR_GAME_LOOP_TIME)
		{
			pWorld->update(WORLD_UPDATE_TIME, renderWindow);
			mSimulatedTimeOfCurrentSample += WORLD_UPDATE_TIME;
		}
	}
	
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
