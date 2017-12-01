#ifndef WORLD_HPP
#define WORLD_HPP


#include "SFML\Graphics.hpp"

#include "Source\Track\Track.hpp"
#include "Source\Car\Car.hpp"

#include <list>



class World
{
private:
	Track mTrack;
	std::list<Car> mListOfCars;

public:
	World();
	World(Track const & track, std::list<Car> const & listOfCars);
	World(World const &) = default;
	World& operator=(World const &) = default;
	~World() = default;

public:
	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

public:
	Track const & getTrackReference() const;
	std::list<Car> const & getCarsReference() const;

};







#endif //WORLD_HPP

