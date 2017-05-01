#ifndef LINE_HPP
#define LINE_HPP

#include "SFML\Graphics.hpp"

#include "myUsefulMath.hpp"
#include "mySFMLVectorFunctions.hpp"


class Line
{
public:
	sf::Vector2f vertex1;
	sf::Vector2f vertex2;


public:
	Line();
	Line(sf::Vector2f const & _vertex1, sf::Vector2f const & _vertex2);

	sf::Vector2f getUnitTangentVector() const;
	sf::Vector2f getUnitNormalVector() const;
	float projectPointOnLine(sf::Vector2f const & point) const;

	bool intersects(Line const & line, sf::Vector2f & intersectionPoint = sf::Vector2f()) const;
	bool contains(sf::Vector2f const & point, float tolerance = 1.0E-05f) const;

};




#endif //LINE_HPP