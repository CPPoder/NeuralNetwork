#include "stdafx.h"
#include "Source\Collision\Line.hpp"


Line::Line()
	: Line(sf::Vector2f(), sf::Vector2f())
{
}

Line::Line(sf::Vector2f const & _vertex1, sf::Vector2f const & _vertex2)
	: vertex1(_vertex1), vertex2(_vertex2)
{
}



sf::Vector2f Line::getUnitTangentVector() const
{
	return mySFML::Simple::normalize(vertex2 - vertex1);
}

sf::Vector2f Line::getUnitNormalVector() const
{
	sf::Vector2f tangent(this->getUnitTangentVector());
	return sf::Vector2f(tangent.y, -tangent.x);
}

float Line::projectPointOnLine(sf::Vector2f const & point) const
{
	return mySFML::Simple::scalarProduct(this->getUnitTangentVector(), point - vertex1);
}


bool Line::intersects(Line const & line, sf::Vector2f & intersectionPoint) const
{
	sf::Vector2f diff1 = this->vertex2 - this->vertex1; //g2 - g1
	sf::Vector2f diff2 = line.vertex2 - line.vertex1; //h2 - h1

	//Suppose:
	//g(s) = g1 + (g2 - g1) * s
	//h(t) = h1 + (h2 - h1) * s
	//Then g(s) = h(t) yields:
	//t = [(g2 - g1).y * (h1 - g1).x - (g2 - g1).x * (h1 - g1).y] / [(g2 - g1).x * (h2 - h1).y - (g2 - g1).y * (h2 - h1).x]
	//s = [(h2 - h1).y * (g1 - h1).x - (h2 - h1).x * (g1 - h1).y] / [(h2 - h1).x * (g2 - g1).y - (h2 - h1).y * (g2 - g1).x] Denominater is -Denom. of t

	float denominator = diff1.x * diff2.y - diff1.y * diff2.x;
	float t = (diff1.y * (line.vertex1 - this->vertex1).x - diff1.x * (line.vertex1 - this->vertex1).y) / denominator;
	float s = (diff2.y * (line.vertex1 - this->vertex1).x - diff2.x * (line.vertex1 - this->vertex1).y) / denominator;

	intersectionPoint = this->vertex1 + diff1 * s;
	return (t >= 0 && t <= 1 && s >= 0 && s <= 1);
}

bool Line::contains(sf::Vector2f const & point, float tolerance) const
{
	sf::Vector2f diffOfVerts = this->vertex2 - this->vertex1;
	sf::Vector2f newPoint = point;
	sf::Vector2f newVert1 = this->vertex1;
	sf::Vector2f newVert2 = this->vertex2;
	sf::Vector2f newDiffOfVerts = diffOfVerts;
	if (myMath::Simple::abs(diffOfVerts.x) < myMath::Simple::abs(diffOfVerts.y))
	{
		newPoint = mySFML::Simple::swapXandY(point);
		newVert1 = mySFML::Simple::swapXandY(this->vertex1);
		newVert2 = mySFML::Simple::swapXandY(this->vertex2);
		newDiffOfVerts = mySFML::Simple::swapXandY(diffOfVerts);
	}
	float m = newDiffOfVerts.y / newDiffOfVerts.x;
	float t = newVert1.y - m * newVert1.x;

	bool liesOnInfiniteLine = (myMath::Simple::abs(m * point.x + t - point.y) < tolerance);
	bool liesBetweenVerts = ((myMath::Simple::min(newVert1.x, newVert2.x) < newPoint.x + tolerance) && (newPoint.x - tolerance < myMath::Simple::max(newVert1.x, newVert2.x)));

	return (liesOnInfiniteLine && liesBetweenVerts);
}


