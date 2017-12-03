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
	//Standard view
	//sf::View raceView(sf::FloatRect(0.f, 0.f, 400.f, 200.f));
	sf::View raceView(sf::FloatRect(0.f, 0.f, 800.f, 400.f));

	//Car following view
	static sf::View usedView = sf::View();
	Car const & car = mListOfCars.front();
	sf::View carView;
	carView.setSize(200.f, 100.f);
	carView.setCenter(car.getPosition() + 1.2f * car.getVelocity() * car.getDirection());
	carView.setRotation(90.f - mySFML::Simple::angleOf(car.getDirection()) / myMath::Const::PIf * 180.f);
	carView.zoom(std::sqrt(0.1f + car.getVelocity() * car.getVelocity() / 1000.f));
	float constexpr mix = 0.95f;
	usedView.setCenter(mix*usedView.getCenter() + (1 - mix)*carView.getCenter());
	usedView.setSize(mix*usedView.getSize() + (1 - mix)*carView.getSize());
	if (std::abs(usedView.getRotation() - carView.getRotation()) <= 180.f)
	{
		usedView.setRotation(mix*usedView.getRotation() + (1 - mix)*carView.getRotation());
	}
	else
	{
		if (carView.getRotation() > usedView.getRotation())
		{
			usedView.setRotation(mix*usedView.getRotation() + (1 - mix)*(carView.getRotation() - 360.f));
		}
		else
		{
			usedView.setRotation(mix*(usedView.getRotation() - 360.f) + (1 - mix)*carView.getRotation());
		}
	}
	//std::cout << usedView.getRotation() << " " << carView.getRotation() << std::endl;

	sf::View originalView(renderWindow->getView());
	//renderWindow->setView(raceView);
	renderWindow->setView(usedView);

	mTrack.render(renderWindow);
	for (auto & car : mListOfCars)
	{
		car.render(renderWindow);
	}

	renderWindow->setView(originalView);
}

void World::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	for (auto & car : mListOfCars)
	{
		car.update(time, renderWindow, this);
	}
}


Track const & World::getTrackReference() const
{
	return mTrack;
}

std::list<Car> const & World::getCarsReference() const
{
	return mListOfCars;
}
