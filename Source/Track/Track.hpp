#ifndef TRACK_HPP
#define TRACK_HPP

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include <iostream>
#include <utility>
#include <list>


typedef std::pair<sf::Vector2f, sf::Vector2f> TrackSegment;

class Car;

class Track
{
private:
	std::list<TrackSegment> mListOfTrackSegments;
	sf::VertexArray mVertexArrayOfTrack;
	sf::Color mTrackColor;

public:
	Track();
	Track(std::list<TrackSegment> const & listOfTrackSegments, sf::Color const & color = sf::Color::White);
	Track(std::list<std::pair<sf::Vector2f, float>> const & listOfPositionsAndWidths, sf::Color const & color = sf::Color::White);
	Track(Track const &) = default;
	Track& operator=(Track const &) = default;
	~Track() = default;

	void setTrack(std::list<TrackSegment> const & listOfTrackSegments);
	void setColor(sf::Color const & color);

	sf::Color getColor() const;

	void render(sf::RenderWindow* renderWindow);

	bool checkCollisionWith(Car const & car) const;

private:
	void refreshVertexArray();
	
private:
	static std::list<TrackSegment> convertIntoListOfTrackSegments(std::list<std::pair<sf::Vector2f, float>> const & listOfPositionsAndWidths);

};





#endif //TRACK_HPP

