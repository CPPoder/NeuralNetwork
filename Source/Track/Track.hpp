#ifndef TRACK_HPP
#define TRACK_HPP

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include "Source\Collision\CollisionArea.hpp"

#include <iostream>
#include <utility>
#include <list>
#include <string>


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
	Track(std::string const & filePath);
	Track(std::list<BorderTrackSegment> const & listOfTrackSegments, sf::Color const & color = sf::Color::White);
	Track(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths, sf::Color const & color = sf::Color::White);
	Track(Track const &) = default;
	Track& operator=(Track const &) = default;
	~Track() = default;

	void setTrack(std::list<BorderTrackSegment> const & listOfTrackSegments);
	void setColor(sf::Color const & color);

	sf::Color getColor() const;
	std::pair<std::list<Line>, std::list<Line>> getListsOfLines() const;
	std::list<CenterWidthTrackSegment> getListOfCenterWidthTrackSegments() const;

	void render(sf::RenderWindow* renderWindow);

	bool checkCollisionWith(Car const & car) const;
	sf::Vector2f calculatePositionInTrackNear(sf::Vector2f const & position) const;

	void saveToFile(std::string const & path) const;
	void loadFromFile(std::string const & path);

	void deformRandomly(unsigned int numberOfDeformations, sf::Vector2f const & sizeOfValidTrackArea, float deformationStep);
	void doubleNumberOfSegments();

private:
	void refreshVertexArray();
	
public:
	bool checkIfTrackIsValid(sf::Vector2f const & sizeOfValidTrackArea) const;
	
private:
	static std::list<BorderTrackSegment> convertIntoListOfBorderTrackSegments(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths);

public:
	static std::list<CenterWidthTrackSegment> constructCircleTrack(sf::Vector2f const & center, float radius, unsigned int pointCount, float width);
	static std::list<CenterWidthTrackSegment> doOneRandomDeformation(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths, float maximalDeformationLength);
	static std::list<CenterWidthTrackSegment> createRandomTrack(unsigned int numberOfDeformations, sf::Vector2f const & sizeOfValidTrackArea);

};





#endif //TRACK_HPP

