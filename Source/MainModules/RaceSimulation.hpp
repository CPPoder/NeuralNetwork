#ifndef RACESIMULATION_HPP
#define RACESIMULATION_HPP

#include "SFML\Graphics.hpp"

#include "Source\Track\Track.hpp"
#include "Source\Car\Car.hpp"
#include "Source\MainModules\World.hpp"

#include <list>


class RaceSimulation
{
public:
	enum class Event
	{
		NONE,
		OPEN_RACE_SIMULATION_MENU
	};

private:
	Event mEvent = Event::NONE;

private:
	World mWorld;

public:
	RaceSimulation();
	~RaceSimulation();

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

	Event pollRaceSimulationEvent();
	void reactOnESC();


};





#endif //RACESIMULATION_HPP

