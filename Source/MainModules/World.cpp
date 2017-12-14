#include "stdafx.h"
#include "Source\MainModules\World.hpp"

#include "Source\Math\myUsefulMath.hpp"
#include "Source\Framework\EventManager.hpp"



World::World()
	: mTrack("./Data/Tracks/test.tr")
{
}


World::World(Track const & track, std::list<Car> const & listOfCars)
	: mTrack(track), mListOfCars(listOfCars)
{
}



void World::render(sf::RenderWindow * renderWindow)
{
	//Get wanted view
	sf::View wantedView = this->getWantedView();

	//Mix wanted view with mUsedView
	float constexpr mix = 0.95f;
	mUsedView.setCenter(mix*mUsedView.getCenter() + (1 - mix)*wantedView.getCenter());
	mUsedView.setSize(mix*mUsedView.getSize() + (1 - mix)*wantedView.getSize());
	if (std::abs(mUsedView.getRotation() - wantedView.getRotation()) <= 180.f)
	{
		mUsedView.setRotation(mix*mUsedView.getRotation() + (1 - mix)*wantedView.getRotation());
	}
	else
	{
		if (wantedView.getRotation() > mUsedView.getRotation())
		{
			mUsedView.setRotation(mix*mUsedView.getRotation() + (1 - mix)*(wantedView.getRotation() - 360.f));
		}
		else
		{
			mUsedView.setRotation(mix*(mUsedView.getRotation() - 360.f) + (1 - mix)*wantedView.getRotation());
		}
	}

	//Set mUsedView for rendering (save original view)
	sf::View originalView(renderWindow->getView());
	renderWindow->setView(mUsedView);

	//Do actual rendering
	mTrack.render(renderWindow);
	for (auto & car : mListOfCars)
	{
		car.render(renderWindow);
	}

	//Restore original view
	renderWindow->setView(originalView);
}

void World::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	for (auto & car : mListOfCars)
	{
		car.update(time, renderWindow, this);
	}
}


void World::clearCars()
{
	mListOfCars.clear();
}

void World::addCar(Car const & car)
{
	mListOfCars.push_back(car);
}


Track const & World::getTrackReference() const
{
	return mTrack;
}

std::list<Car> const & World::getCarsReference() const
{
	return mListOfCars;
}

sf::View World::getUsedView() const
{
	return mUsedView;
}


sf::View World::getWantedView() const
{
	//Standard view
	sf::View raceView(sf::FloatRect(0.f, 0.f, 800.f, 400.f));

	//Car following view
	Car const & car = mListOfCars.front();
	sf::View carView;
	carView.setSize(200.f, 100.f);
	carView.setCenter(car.getPosition() + 1.2f * car.getVelocity() * car.getDirection());
	carView.setRotation(90.f - mySFML::Simple::angleOf(car.getDirection()) / myMath::Const::PIf * 180.f);
	carView.zoom(std::sqrt(0.1f + car.getVelocity() * car.getVelocity() / 1000.f));

	//Return wanted view (Either raceView or carView)
	return carView;
}

void World::setUsedViewToWantedView()
{
	mUsedView = this->getWantedView();
}
