#ifndef TRACK_HPP
#define TRACK_HPP

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include "Source\Collision\CollisionArea.hpp"

#include <iostream>
#include <utility>
#include <list>
#include <string>
#include <functional>

#include "Source\Track\BorderTrackBase.hpp"
#include "Source\Track\CenterTrackBase.hpp"


class Car;

class Track
{
private:
	BorderTrackBase mBorderTrack;
	sf::FloatRect mValidTrackArea;

	sf::VertexArray mVertexArrayOfTrack;
	sf::Color mTrackColor;

public:
	Track() = delete;
	Track(std::string const & filePath);
	Track(BorderTrackBase const & borderTrackBase, sf::FloatRect const & validTrackArea, sf::Color const & color = sf::Color::White);
	Track(CenterTrackBase const & centerTrackBase, sf::FloatRect const & validTrackArea, sf::Color const & color = sf::Color::White);
	Track(Track const &) = default;
	Track& operator=(Track const &) = default;
	~Track() = default;

	void setTrack(BorderTrackBase const & borderTrackBase, sf::FloatRect const & validTrackArea);
	void setTrack(CenterTrackBase const & centerTrackBase, sf::FloatRect const & validTrackArea);
	void setColor(sf::Color const & color);
	void setValidTrackArea(sf::FloatRect const & floatRectOfValidTrackArea);

	sf::Color getColor() const;
	sf::FloatRect getValidTrackArea() const;
	unsigned int getNumberOfSegments() const;
	BorderTrackBase getBorderTrackBase() const;
	CenterTrackBase getCenterTrackBase() const;
	std::pair<std::list<Line>, std::list<Line>> getListsOfLines() const;

	void render(sf::RenderWindow* renderWindow);

	bool checkCollisionWith(Car const & car) const;
	sf::Vector2f calculatePositionInTrackNear(sf::Vector2f const & position) const;

	void saveToFile(std::string const & path) const;
	void loadFromFile(std::string const & path);

	void deformRandomly(unsigned int numberOfDeformations, float deformationStep);
	void doOneRandomDeformation(float deformationLength);
	void doubleNumberOfSegments();

private:
	void refreshState();
	
public:
	bool checkIfTrackIsValid() const;

public:
	static Track constructCircleTrack(sf::Vector2f const & center, float radius, unsigned int pointCount, float width, sf::FloatRect const & validTrackArea);

};





#endif //TRACK_HPP

