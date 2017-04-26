#ifndef RACESIMULATION_HPP
#define RACESIMULATION_HPP

#include "SFML\Graphics.hpp"

#include "Source\Track\Track.hpp"


class RaceSimulation
{
private:
	Track mTrack;

public:
	RaceSimulation();
	~RaceSimulation();

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

private:
	static std::list<std::pair<sf::Vector2f, float>> constructCircleTrack();



};





#endif //RACESIMULATION_HPP

