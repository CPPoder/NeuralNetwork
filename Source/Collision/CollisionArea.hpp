#ifndef COLLISIONAREA_HPP
#define COLLISIONAREA_HPP

#include "SFML\Graphics.hpp"

#include <list>

#include "mySFMLVectorFunctions.hpp"

#include "Source\Collision\Line.hpp"


class CollisionArea
{
private:
	std::list<sf::Shape*> mListOfShapes;

public:
	CollisionArea();
	CollisionArea(CollisionArea const & collisionArea);
	CollisionArea(std::list<sf::Shape*> listOfShapes);
	CollisionArea(sf::Shape* shape);
	~CollisionArea();

	void move(sf::Vector2f movement);
	void rotateAround(sf::Vector2f origin, float angle);

	void render(sf::RenderWindow* renderWindow) const;

	void set(CollisionArea const & collisionArea);
	void set(std::list<sf::Shape*> listOfShapes);
	void addShape(sf::Shape* shape);

	bool checkCollisionWith(sf::Shape* shape) const;
	bool checkCollisionWith(Line const & line) const;

private:
	static bool tryToConvertToRectangleShape(sf::Shape* input, sf::RectangleShape* & output);
	static bool tryToConvertToCircleShape(sf::Shape* input, sf::CircleShape* & output);
	static bool checkIfInputIsRectangleShapeOrCircleShape(sf::Shape* input);

	static bool checkCollisionBetween(sf::Shape* s1, sf::Shape* s2);
	static bool checkCollisionBetween(sf::Shape* s, Line l);
	static bool checkCollisionBetween(sf::CircleShape* c1, sf::CircleShape* c2);
	static bool checkCollisionBetween(sf::CircleShape* c, sf::RectangleShape* r);
	static bool checkCollisionBetween(sf::RectangleShape* r1, sf::RectangleShape* r2);
	static bool checkCollisionBetween(sf::CircleShape* c, Line const & l);
	static bool checkCollisionBetween(sf::RectangleShape* r, Line const & l);

	static sf::Shape* setOriginCorrect(sf::Shape* s);
	static sf::CircleShape* setOriginCentered(sf::CircleShape* c);
	static sf::RectangleShape* setOriginTopLeft(sf::RectangleShape* r);

	static std::vector<sf::Vector2f> getCornerPositionsOf(sf::RectangleShape* r);

};



#endif //COLLISIONAREA_HPP

