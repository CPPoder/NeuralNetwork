#ifndef RACESIMULATION_HPP
#define RACESIMULATION_HPP

#include "SFML\Graphics.hpp"

#include "Source\Track\Track.hpp"
#include "Source\Car\Car.hpp"

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
	sf::Vector2f mSize;

private:
	Track mTrack;

	std::list<Car> mListOfCars;

public:
	RaceSimulation() = delete;
	RaceSimulation(sf::Vector2f const & size);
	~RaceSimulation();

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

	Event pollRaceSimulationEvent();
	void reactOnESC();

public:
	Track const & getTrackReference() const;


};





#endif //RACESIMULATION_HPP

