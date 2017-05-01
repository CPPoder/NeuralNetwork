#include "stdafx.h"
#include "Source\Collision\CollisionArea.hpp"

#include <functional>
#include <array>


//Constructors
CollisionArea::CollisionArea()
{
}
CollisionArea::CollisionArea(CollisionArea const & collisionArea)
{
	this->set(collisionArea);
}
CollisionArea::CollisionArea(std::list<sf::Shape*> listOfShapes)
{
	this->set(listOfShapes);
}
CollisionArea::CollisionArea(sf::Shape* shape)
{
	this->addShape(shape);
}

//Destructor
CollisionArea::~CollisionArea()
{
	for (auto shape : mListOfShapes)
	{
		delete shape;
		shape = nullptr;
	}
	mListOfShapes.clear();
}


//Movement Setter
void CollisionArea::move(sf::Vector2f movement)
{
	for (auto shape : mListOfShapes)
	{
		shape->move(movement);
	}
}
void CollisionArea::rotateAround(sf::Vector2f origin, float angle)
{
	for (auto shape : mListOfShapes)
	{
		shape->setOrigin(origin);
		shape->rotate(angle);
		this->setOriginCorrect(shape);
	}
}


//Render
void CollisionArea::render(sf::RenderWindow* renderWindow) const
{
	for (auto shape : mListOfShapes)
	{
		renderWindow->draw(*shape);
	}
}


//Shape Setter
void CollisionArea::set(CollisionArea const & collisionArea)
{
	this->set(collisionArea.mListOfShapes);
}
void CollisionArea::set(std::list<sf::Shape*> listOfShapes)
{
	for (auto shape : listOfShapes)
	{
		this->addShape(shape);
	}
}
void CollisionArea::addShape(sf::Shape* shape)
{
	sf::RectangleShape* rectShapePointer = nullptr;
	sf::CircleShape* circleShapePointer = nullptr;
	if (this->tryToConvertToRectangleShape(shape, rectShapePointer))
	{
		mListOfShapes.push_back(this->setOriginTopLeft(new sf::RectangleShape(*rectShapePointer))); //SetOriginTopLeft does nothing at the moment!!!
	}
	if (this->tryToConvertToCircleShape(shape, circleShapePointer))
	{
		mListOfShapes.push_back(this->setOriginCentered(new sf::CircleShape(*circleShapePointer)));
	}
}


//Collision Check
bool CollisionArea::checkCollisionWith(sf::Shape* shape) const
{
	for (auto s : mListOfShapes)
	{
		if (this->checkCollisionBetween(s, shape))
		{
			return true;
		}
	}
	return false;
}
bool CollisionArea::checkCollisionWith(Line const & line) const
{
	for (auto s : mListOfShapes)
	{
		if (this->checkCollisionBetween(s, line))
		{
			return true;
		}
	}
	return false;
}



//Internal Functions
bool CollisionArea::tryToConvertToRectangleShape(sf::Shape* input, sf::RectangleShape* & output)
{
	sf::RectangleShape* rectangleShapePointer = dynamic_cast<sf::RectangleShape*>(input);
	if (rectangleShapePointer == nullptr)
	{
		output = nullptr;
		return false;
	}
	else
	{
		output = rectangleShapePointer;
		return true;
	}
}
bool CollisionArea::tryToConvertToCircleShape(sf::Shape* input, sf::CircleShape* & output)
{
	sf::CircleShape* circleShapePointer = dynamic_cast<sf::CircleShape*>(input);
	if (circleShapePointer == nullptr)
	{
		output = nullptr;
		return false;
	}
	else
	{
		output = circleShapePointer;
		return true;
	}
}
bool CollisionArea::checkIfInputIsRectangleShapeOrCircleShape(sf::Shape* input)
{
	return ((dynamic_cast<sf::CircleShape*>(input) != nullptr) || (dynamic_cast<sf::RectangleShape*>(input) != nullptr));
}


bool CollisionArea::checkCollisionBetween(sf::Shape* s1, sf::Shape* s2)
{
	sf::CircleShape* c1 = nullptr;
	sf::CircleShape* c2 = nullptr;
	sf::RectangleShape* r1 = nullptr;
	sf::RectangleShape* r2 = nullptr;
	if (tryToConvertToCircleShape(s1, c1))
	{
		if (tryToConvertToCircleShape(s2, c2))
		{
			return checkCollisionBetween(c1, c2);
		}
		if (tryToConvertToRectangleShape(s2, r2))
		{
			return checkCollisionBetween(c1, r2);
		}
	}
	if (tryToConvertToRectangleShape(s1, r1))
	{
		if (tryToConvertToCircleShape(s2, c2))
		{
			return checkCollisionBetween(c2, r1);
		}
		if (tryToConvertToRectangleShape(s2, r2))
		{
			return checkCollisionBetween(r1, r2);
		}
	}
	return false;
}
bool CollisionArea::checkCollisionBetween(sf::Shape* s, Line l)
{
	sf::RectangleShape* r = nullptr;
	sf::CircleShape* c = nullptr;
	if (tryToConvertToRectangleShape(s, r))
	{
		return checkCollisionBetween(r, l);
	}
	if (tryToConvertToCircleShape(s, c))
	{
		return checkCollisionBetween(c, l);
	}
	return false;
}

bool CollisionArea::checkCollisionBetween(sf::CircleShape* c1, sf::CircleShape* c2)
{
	return (mySFML::Simple::lengthOf(c1->getPosition() - c2->getPosition()) < (c1->getRadius() + c2->getRadius()));
}
bool CollisionArea::checkCollisionBetween(sf::CircleShape* c, sf::RectangleShape* r)
{
	//Check 3 things: Is c completely inside r? Does c overlap with any corner of r? Does c overlap with any edge of r?

	//Extract data
	sf::FloatRect localBounds(r->getLocalBounds());
	sf::Vector2f localBoundsTopLeft(localBounds.left, localBounds.top);
	sf::Vector2f localBoundsSize(localBounds.width, localBounds.height);
	sf::Transform trafo(r->getTransform());
	std::array<sf::Vector2f, 4u> arrayOfTransformedPoints;
	arrayOfTransformedPoints.at(0) = trafo.transformPoint(localBoundsTopLeft);
	arrayOfTransformedPoints.at(1) = trafo.transformPoint(localBoundsTopLeft + sf::Vector2f(0.f, localBoundsSize.y));
	arrayOfTransformedPoints.at(2) = trafo.transformPoint(localBoundsTopLeft + localBoundsSize);
	arrayOfTransformedPoints.at(3) = trafo.transformPoint(localBoundsTopLeft + sf::Vector2f(localBoundsSize.x, 0.f));
	std::array<Line, 4u> arrayOfLines;
	arrayOfLines.at(0) = Line(arrayOfTransformedPoints.at(0), arrayOfTransformedPoints.at(1));
	arrayOfLines.at(1) = Line(arrayOfTransformedPoints.at(1), arrayOfTransformedPoints.at(2));
	arrayOfLines.at(2) = Line(arrayOfTransformedPoints.at(2), arrayOfTransformedPoints.at(3));
	arrayOfLines.at(3) = Line(arrayOfTransformedPoints.at(3), arrayOfTransformedPoints.at(0));
	float radius = c->getRadius();
	sf::Vector2f center = c->getPosition();

	//Check if c overlaps with any corner
	for (auto point : arrayOfTransformedPoints)
	{
		if (mySFML::Simple::lengthOf(center - point) < radius)
		{
			return true;
		}
	}

	//Check if c overlaps with any edge
	for (auto edge : arrayOfLines)
	{
		if (checkCollisionBetween(c, edge))
		{
			return true;
		}
	}

	//Check if c is completely inside of r
	sf::Transform inverseTrafo(r->getInverseTransform());
	sf::Vector2f centerAfterInverseTrafo(inverseTrafo.transformPoint(center));
	if (r->getLocalBounds().contains(centerAfterInverseTrafo))
	{
		return true;
	}

	return false;
}
bool CollisionArea::checkCollisionBetween(sf::RectangleShape* r1, sf::RectangleShape* r2)
{
	std::vector<sf::Vector2f> cornerPositions1 = getCornerPositionsOf(r1);
	std::vector<sf::Vector2f> cornerPositions2 = getCornerPositionsOf(r2);

	std::function<bool(Line const &, std::vector<sf::Vector2f> const &, std::vector<sf::Vector2f> const &)> checkIfProjectionsOntoLineOverlap = 
		[](Line const & line, std::vector<sf::Vector2f> const & cornerPositions1, std::vector<sf::Vector2f> const & cornerPositions2) -> bool
	{
		float r2Min, r2Max;
		bool minMax2Initialized = false;
		for (auto point : cornerPositions2)
		{
			float projection = line.projectPointOnLine(point);
			if (minMax2Initialized)
			{
				r2Min = myMath::Simple::min(r2Min, projection);
				r2Max = myMath::Simple::max(r2Max, projection);
			}
			else
			{
				r2Min = projection;
				r2Max = projection;
				minMax2Initialized = true;
			}
		}
		float r1Min, r1Max;
		bool minMax1Initialized = false;
		for (auto point : cornerPositions1)
		{
			float projection = line.projectPointOnLine(point);
			if (minMax1Initialized)
			{
				r1Min = myMath::Simple::min(r1Min, projection);
				r1Max = myMath::Simple::max(r1Max, projection);
			}
			else
			{
				r1Min = projection;
				r1Max = projection;
				minMax1Initialized = true;
			}
		}
		float a;
		float b;

		return myMath::Simple::checkIfIntervalsOverlap(r1Min, r1Max, r2Min, r2Max, a, b);
	};

	if (!checkIfProjectionsOntoLineOverlap(Line(cornerPositions1.at(0), cornerPositions1.at(1)), cornerPositions1, cornerPositions2))
	{
		return false;
	}
	else if (!checkIfProjectionsOntoLineOverlap(Line(cornerPositions1.at(1), cornerPositions1.at(2)), cornerPositions1, cornerPositions2))
	{
		return false;
	}
	else if (!checkIfProjectionsOntoLineOverlap(Line(cornerPositions2.at(0), cornerPositions2.at(1)), cornerPositions1, cornerPositions2))
	{
		return false;
	}
	else if (!checkIfProjectionsOntoLineOverlap(Line(cornerPositions2.at(1), cornerPositions2.at(2)), cornerPositions1, cornerPositions2))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CollisionArea::checkCollisionBetween(sf::CircleShape* c, Line const & l) //Ignores Scale and Rotation
{
	//Check if one or both points of l lie in c
	sf::Vector2f center = c->getPosition();
	sf::Vector2f vertex1 = l.vertex1;
	sf::Vector2f vertex2 = l.vertex2;
	float r = c->getRadius();
	if (mySFML::Simple::lengthOf(center - vertex1) < r || mySFML::Simple::lengthOf(center - vertex2) < r)
	{
		return true;
	}
	else
	{
		//Calculate transformed vertices, such that the circle is centered
		sf::Vector2f newVertex1 = vertex1 - center;
		sf::Vector2f newVertex2 = vertex2 - center;
		sf::Vector2f diffOfNewVerts = newVertex2 - newVertex1;
		if (myMath::Simple::abs(diffOfNewVerts.x) < myMath::Simple::abs(diffOfNewVerts.y))
		{
			//Swap x and y in order to get a safe m calculation (Circle is symmetric, thus not affected!)
			diffOfNewVerts = mySFML::Simple::swapXandY(diffOfNewVerts);
			diffOfNewVerts = mySFML::Simple::swapXandY(newVertex1);
			diffOfNewVerts = mySFML::Simple::swapXandY(newVertex2);
		}
		float m = diffOfNewVerts.y / diffOfNewVerts.x;
		float t = newVertex1.y - m * newVertex1.x;

		//Check if line goes through circleShape
		float discriminant = 4 * m * m * t * t - 4 * (m * m + 1) * (t * t - r * r);
		if (discriminant < 0.f)
		{
			return false;
		}
		else
		{
			//Check if the solutions are not only solutions of the infinite line
			Line newLine(newVertex1, newVertex2);
			float solution1x = (-2 * m * t + sqrt(discriminant)) / (2 * (m * m + 1));
			float solution2x = (-2 * m * t - sqrt(discriminant)) / (2 * (m * m + 1));
			sf::Vector2f sol1(solution1x, m * solution1x + t);
			sf::Vector2f sol2(solution2x, m * solution2x + t);
			if (newLine.contains(sol1) || newLine.contains(sol2))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
bool CollisionArea::checkCollisionBetween(sf::RectangleShape* r, Line const & l)
{
	sf::Transform inverseTrafo(r->getInverseTransform());
	sf::FloatRect localBounds = r->getLocalBounds();
	bool someEndpointInside = (localBounds.contains(inverseTrafo.transformPoint(l.vertex1)) || localBounds.contains(inverseTrafo.transformPoint(l.vertex2)));
	if (someEndpointInside)
	{
		return true;
	}
	auto cornerPositions = getCornerPositionsOf(r);
	Line line1(cornerPositions.at(0), cornerPositions.at(1));
	if (line1.intersects(l))
	{
		return true;
	}
	Line line2(cornerPositions.at(1), cornerPositions.at(2));
	if (line2.intersects(l))
	{
		return true;
	}
	Line line3(cornerPositions.at(2), cornerPositions.at(3));
	if (line3.intersects(l))
	{
		return true;
	}
	Line line4(cornerPositions.at(3), cornerPositions.at(0));
	if (line4.intersects(l))
	{
		return true;
	}
	return false;
}

sf::Shape* CollisionArea::setOriginCorrect(sf::Shape* s)
{
	sf::RectangleShape* r = nullptr;
	sf::CircleShape* c = nullptr;
	if (tryToConvertToRectangleShape(s, r))
	{
		return setOriginTopLeft(r);
	}
	if (tryToConvertToCircleShape(s, c))
	{
		return setOriginCentered(c);
	}
}
sf::CircleShape* CollisionArea::setOriginCentered(sf::CircleShape* c)
{
	sf::FloatRect globalBounds = c->getGlobalBounds();
	c->setOrigin(globalBounds.left + globalBounds.width / 2.f, globalBounds.top + globalBounds.height / 2.f);
	return c;
}
sf::RectangleShape* CollisionArea::setOriginTopLeft(sf::RectangleShape* r) //I don't see anymore why I would need this! Thus, does nothing at the moment!!!
{
	return r;
}

std::vector<sf::Vector2f> CollisionArea::getCornerPositionsOf(sf::RectangleShape* r)
{
	sf::Transform trafo = r->getTransform();
	sf::FloatRect localBounds = r->getLocalBounds();
	sf::Vector2f topLeft = sf::Vector2f(localBounds.left, localBounds.top);
	sf::Vector2f size = sf::Vector2f(localBounds.width, localBounds.height);

	std::vector<sf::Vector2f> cornerPositions;
	cornerPositions.reserve(4);

	cornerPositions.push_back(trafo.transformPoint(topLeft));
	cornerPositions.push_back(trafo.transformPoint(topLeft + sf::Vector2f(0.f, size.y)));
	cornerPositions.push_back(trafo.transformPoint(topLeft + size));
	cornerPositions.push_back(trafo.transformPoint(topLeft + sf::Vector2f(size.x, 0.f)));

	return cornerPositions;
}