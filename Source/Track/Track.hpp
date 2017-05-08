#ifndef TRACK_HPP
#define TRACK_HPP

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include "Source\Collision\CollisionArea.hpp"

#include <iostream>
#include <utility>
#include <list>


typedef std::pair<sf::Vector2f, sf::Vector2f> BorderTrackSegment;
typedef std::pair<sf::Vector2f, float> CenterWidthTrackSegment;

class Car;

class Track
{
private:
	std::list<BorderTrackSegment> mListOfTrackSegments;
	sf::VertexArray mVertexArrayOfTrack;
	sf::Color mTrackColor;

public:
	Track();
	Track(std::list<BorderTrackSegment> const & listOfTrackSegments, sf::Color const & color = sf::Color::White);
	Track(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths, sf::Color const & color = sf::Color::White);
	Track(Track const &) = default;
	Track& operator=(Track const &) = default;
	~Track() = default;

	void setTrack(std::list<BorderTrackSegment> const & listOfTrackSegments);
	void setColor(sf::Color const & color);

	sf::Color getColor() const;
	std::pair<std::list<Line>, std::list<Line>> getListsOfLines() const;

	void render(sf::RenderWindow* renderWindow);

	bool checkCollisionWith(Car const & car) const;
	sf::Vector2f calculatePositionInTrackNear(sf::Vector2f const & position) const;

private:
	void refreshVertexArray();
	
private:
	bool checkIfTrackIsValid() const;
	
private:
	static std::list<BorderTrackSegment> convertIntoListOfBorderTrackSegments(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths);

public:
	static std::list<CenterWidthTrackSegment> constructCircleTrack(sf::Vector2f const & center, float radius, unsigned int pointCount, float width);
	static std::list<CenterWidthTrackSegment> randomlyDeformTrack(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths);
	static std::list<CenterWidthTrackSegment> createRandomTrack();

};





#endif //TRACK_HPP

