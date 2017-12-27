#include "stdafx.h"
#include "Source\MainModules\LearnModule.hpp"


LearnModule::LearnModule()
{
	Track track("./Data/Tracks/learning.tr");
	pWorld = new World(track, {});
	pWorld->setWantedViewToTrackView(sf::FloatRect(0.f, 0.f, 400.f, 200.f));
}

LearnModule::~LearnModule()
{
	mBestNetBrain.saveToFile("./Data/Nets/learnFromRandom.net");
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
	sf::Time const SIMULATION_TIME = sf::seconds(200.f); //This could be made varying!
	sf::Time const INTERRUPT_FOR_GAME_LOOP_TIME = sf::seconds(1.f / 50.f);
	sf::Time const WORLD_UPDATE_TIME = sf::seconds(1.f / 20.f); //This can probably be chosen much bigger: e.g. 1/10. This would increase the learning rate by a factor of 5 and would probably have no impact on the driving behaviour! (Humans change there behaviour almost only every second!) However, short times yield big jumps in update procedure! Better choose not to large times!

	//Initialize first sample
	if (mFirstSample && pWorld->getCarsReference().empty())
	{
		sf::Vector2f carPos = pWorld->getTrackReference().getStartPosition();
		mBestNetBrain = NetBrain(); //Load here from some file later!
		//mBestNetBrain.setEntriesRandom();
		mBestNetBrain.loadFromFile("./Data/Nets/learnFromRandom.net");
		mCurrentNetBrain = mBestNetBrain;
		Car neuralNetCar(carPos, pWorld->getTrackReference().getForwardDirectionAt(carPos), 0.f, mCurrentNetBrain);
		pWorld->addCar(neuralNetCar);
		pWorld->setUsedViewToWantedView();
		mSimulatedTimeOfCurrentSample = sf::Time::Zero;
		mTimeWithoutMovement = sf::Time::Zero;
		mCurrentIntegratedPath = 0.f;
	}

	if ((mSimulatedTimeOfCurrentSample > SIMULATION_TIME) || (mTimeWithoutMovement > sf::seconds(10.f))) //Rate, accept/discard, initialize new mutated sample
	{
		//Reset mSimulatedTimeOfCurrentSample
		mSimulatedTimeOfCurrentSample = sf::Time::Zero;
		mTimeWithoutMovement = sf::Time::Zero;

		//Rate
		float rate = pWorld->getTrackReference().getDistanceFromStartTo(pWorld->getCarsReference().front().getPosition());
		rate *= myMath::Simple::sign(mCurrentIntegratedPath - 10.f);
		rate = mCurrentIntegratedPath;
		mCurrentIntegratedPath = 0.f;
		std::cout << "Rate: " << rate << "/" << mBestRate << std::endl;
		//Accept, Discard
		if (mFirstSample)
		{
			mFirstSample = false;
			mBestRate = rate;
		}
		else
		{
			float constexpr epsilon = 0.001f;
			if (rate + epsilon > mBestRate)
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
			Car const & firstCar = pWorld->getCarsReference().front();
			mCurrentIntegratedPath += mySFML::Simple::scalarProduct(mySFML::Simple::normalize(pWorld->getTrackReference().getForwardDirectionAt(firstCar.getPosition())), firstCar.getVelocity() * firstCar.getDirection() * WORLD_UPDATE_TIME.asSeconds());
			if (firstCar.getVelocity() < 0.1f)
			{
				mTimeWithoutMovement += WORLD_UPDATE_TIME;
			}
			else
			{
				mTimeWithoutMovement = sf::Time::Zero;
			}
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
