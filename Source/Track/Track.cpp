#include "stdafx.h"
#include "Source\Track\Track.hpp"

#include "Source\Car\Car.hpp"
#include "Source\Collision\CollisionArea.hpp"

#include <array>
#include <functional>
#include <fstream>
#include <sstream>


//////////////
//Constructors

//Constructor from File
Track::Track(std::string const & filePath)
{
	if (this->loadFromFile(filePath))
	{
		mTrackIsInitialized = true;
	}
	this->setColor(sf::Color::White);
}

//Constructor from BorderTrackBase
Track::Track(BorderTrackBase const & borderTrackBase, sf::Vector2f const & startPosition, bool trackDirectionIsForwards, sf::Color const & color)
{
	mTrackIsInitialized = true;
	this->setTrack(borderTrackBase);
	this->setStartPosition(startPosition);
	this->setTrackDirectionIsForwards(trackDirectionIsForwards);
	this->setColor(color);
}

//Constructor from CenterTrackBase
Track::Track(CenterTrackBase const & centerTrackBase, sf::Vector2f const & startPosition, bool trackDirectionIsForwards, sf::Color const & color)
{
	mTrackIsInitialized = true;
	this->setTrack(centerTrackBase);
	this->setStartPosition(startPosition);
	this->setTrackDirectionIsForwards(trackDirectionIsForwards);
	this->setColor(color);
}


////////
//Render

//Render
void Track::render(sf::RenderWindow* renderWindow, unsigned int options)
{
	if ((options & TrackRenderOptions::DRAW_AREA) != 0u)
	{
		renderWindow->draw(mVertexArrayOfTrack);
	}
	if ((options & TrackRenderOptions::DRAW_SEGMENT_LINES) != 0u)
	{
		this->renderSegmentLines(renderWindow);
	}
	if ((options & TrackRenderOptions::DRAW_BORDER_CIRCLES) != 0u)
	{
		this->renderBorderCircles(renderWindow);
	}
	if ((options & TrackRenderOptions::DRAW_CENTER_CIRCLES) != 0u)
	{
		this->renderCenterCircles(renderWindow);
	}
}

void Track::renderSegmentLines(sf::RenderWindow* renderWindow)
{
	sf::Color const segmentLinesColor = sf::Color::Black;
	sf::VertexArray vertexArrayOfSegmentLines(sf::PrimitiveType::Lines);
	for (auto const & trackSegment : mBorderTrack.getListOfBorderTrackSegments())
	{
		vertexArrayOfSegmentLines.append(sf::Vertex(trackSegment.first, segmentLinesColor));
		vertexArrayOfSegmentLines.append(sf::Vertex(trackSegment.second, segmentLinesColor));
	}
	if (!mBorderTrack.getListOfBorderTrackSegments().empty())
	{
		vertexArrayOfSegmentLines.append(sf::Vertex(mBorderTrack.getListOfBorderTrackSegments().front().first, segmentLinesColor));
		vertexArrayOfSegmentLines.append(sf::Vertex(mBorderTrack.getListOfBorderTrackSegments().front().second, segmentLinesColor));
	}
	renderWindow->draw(vertexArrayOfSegmentLines);
}

void Track::renderBorderCircles(sf::RenderWindow* renderWindow)
{
	float constexpr radius = 1.5f;
	sf::Color const color = sf::Color(128, 128, 128);
	sf::CircleShape circleShape(radius, 10u);
	circleShape.setOrigin(radius, radius);
	circleShape.setFillColor(color);
	for (auto const & borderTrackSegment : mBorderTrack)
	{
		circleShape.setPosition(borderTrackSegment.first);
		renderWindow->draw(circleShape);
		circleShape.setPosition(borderTrackSegment.second);
		renderWindow->draw(circleShape);
	}
}

void Track::renderCenterCircles(sf::RenderWindow* renderWindow)
{
	float constexpr radius = 1.5f;
	sf::Color const color = sf::Color(64, 64, 64);
	sf::CircleShape circleShape(radius, 10u);
	circleShape.setOrigin(radius, radius);
	circleShape.setFillColor(color);
	CenterTrackBase centerTrackBase(mBorderTrack.getCenterTrackBase());
	for (auto const & centerTrackSegment : centerTrackBase)
	{
		circleShape.setPosition(centerTrackSegment.first);
		renderWindow->draw(circleShape);
	}
}


////////
//Setter

void Track::setTrack(BorderTrackBase const & borderTrackBase)
{
	mBorderTrack = borderTrackBase;
	this->refreshState();
}

void Track::setTrack(CenterTrackBase const & centerTrackBase)
{
	mBorderTrack = centerTrackBase.getBorderTrackBase();
	this->refreshState();
}

void Track::setStartPosition(sf::Vector2f const & startPosition)
{
	mStartPosition = startPosition;
}

void Track::setTrackDirectionIsForwards(bool isForwards)
{
	mTrackDirectionIsForwards = isForwards;
}

void Track::setColor(sf::Color const & color)
{
	mTrackColor = color;
	this->refreshState();
}



////////
//Getter

sf::Color Track::getColor() const
{
	return mTrackColor;
}

unsigned int Track::getNumberOfSegments() const
{
	return mBorderTrack.getListOfBorderTrackSegments().size();
}

BorderTrackBase Track::getBorderTrackBase() const
{
	return mBorderTrack;
}

CenterTrackBase Track::getCenterTrackBase() const
{
	return mBorderTrack.getCenterTrackBase();
}

std::pair<std::list<Line>, std::list<Line>> Track::getListsOfLines() const
{
	//Define Task
	std::function<std::pair<Line, Line>(BorderTrackSegment const &, BorderTrackSegment const &)> linePairGetter =
		[](BorderTrackSegment const & segment1, BorderTrackSegment const & segment2) -> std::pair<Line, Line>
	{
		return std::make_pair(Line(segment1.first, segment2.first), Line(segment1.second, segment2.second));
	};

	//Use Task for all Sectors
	std::list<std::pair<Line, Line>> listOfLinePairs = mBorderTrack.calculateForAllSectorsAndPutIntoList(linePairGetter);

	//Transform listOfPairs into pairOfLists
	std::list<Line> leftLines;
	std::list<Line> rightLines;
	for (auto const & linePair : listOfLinePairs)
	{
		leftLines.push_back(linePair.first);
		rightLines.push_back(linePair.second);
	}

	//Return Result
	return std::make_pair(std::move(leftLines), std::move(rightLines));
}

bool Track::getIfTrackIsWellInitialized() const
{
	return mTrackIsInitialized;
}

sf::FloatRect Track::getBounds() const
{
	sf::Vector2f min = mBorderTrack.cbegin()->first;
	sf::Vector2f max = mBorderTrack.cbegin()->first;
	for (auto const & pair : mBorderTrack)
	{
		if (pair.first.x < min.x) min.x = pair.first.x;
		if (pair.first.y < min.y) min.y = pair.first.y;
		if (pair.first.x > max.x) max.x = pair.first.x;
		if (pair.first.y > max.y) max.y = pair.first.y;

		if (pair.second.x < min.x) min.x = pair.second.x;
		if (pair.second.y < min.y) min.y = pair.second.y;
		if (pair.second.x > max.x) max.x = pair.second.x;
		if (pair.second.y > max.y) max.y = pair.second.y;
	}
	return sf::FloatRect(min.x, min.y, max.x - min.x, max.y - min.y);
}

bool Track::getIfTrackDirectionIsForwards() const
{
	return mTrackDirectionIsForwards;
}

sf::Vector2f Track::getForwardDirectionAt(sf::Vector2f const & position) const
{
	//Find nearest segment
	bool first = true;
	float smallestDist;
	BorderTrackBase::const_iterator nearestSegment;
	for (BorderTrackBase::const_iterator it = mBorderTrack.cbegin(); it != mBorderTrack.cend(); ++it)
	{
		sf::Vector2f center = mySFML::Simple::meanVector(it->first, it->second);
		if (first)
		{
			first = false;
			nearestSegment = it;
			smallestDist = mySFML::Simple::lengthOf(position - center);
		}
		else
		{
			float newDist = mySFML::Simple::lengthOf(position - center);
			if (newDist < smallestDist)
			{
				smallestDist = newDist;
				nearestSegment = it;
			}
		}
	}

	//Depending on mTrackDirectionIsForwards take either next or previous segment as second segment
	BorderTrackBase::const_iterator secondSegment = nearestSegment;
	if (mTrackDirectionIsForwards)
	{
		if (++secondSegment == mBorderTrack.cend())
		{
			secondSegment = mBorderTrack.cbegin();
		}
	}
	else
	{
		if (secondSegment == mBorderTrack.cbegin())
		{
			secondSegment = --mBorderTrack.cend();
		}
		else
		{
			--secondSegment;
		}
	}

	//Calculate forward direction
	sf::Vector2f firstPos = mySFML::Simple::meanVector(nearestSegment->first, nearestSegment->second);
	sf::Vector2f secondPos = mySFML::Simple::meanVector(secondSegment->first, secondSegment->second);
	return mySFML::Simple::normalize(secondPos - firstPos);
}

sf::Vector2f Track::getStartPosition() const
{
	return mStartPosition;
}



////////////////////
//Collision Checking

//Check for Collision with Car
bool Track::checkCollisionWith(Car const & car) const
{
	//Get Array of Car Lines
	sf::VertexArray const & carVertexArray = car.getVertexArrayReference();
	std::array<Line, 4u> arrayOfCarLines{
		Line(carVertexArray[0].position, carVertexArray[1].position),
		Line(carVertexArray[1].position, carVertexArray[2].position),
		Line(carVertexArray[2].position, carVertexArray[3].position),
		Line(carVertexArray[3].position, carVertexArray[0].position)
	};

	//Get List of Track Lines
	std::pair<std::list<Line>, std::list<Line>> pairOfLineLists = this->getListsOfLines();
	std::list<Line> listOfTrackLines1 = pairOfLineLists.first;
	std::list<Line> listOfTrackLines2 = pairOfLineLists.second;

	//Check for Intersections
	for (auto const & carLine : arrayOfCarLines)
	{
		for (auto const & trackLine : listOfTrackLines1)
		{
			if (carLine.intersects(trackLine))
			{
				return true;
			}
		}
		for (auto const & trackLine : listOfTrackLines2)
		{
			if (carLine.intersects(trackLine))
			{
				return true;
			}
		}
	}
	return false;
}


std::list<sf::Vector2f> Track::getIntersectionsWithBorder(Line const & line) const
{
	std::list<sf::Vector2f> intersections;
	sf::Vector2f placeholder;

	//Get List of Track Lines
	std::pair<std::list<Line>, std::list<Line>> pairOfLineLists = this->getListsOfLines();
	std::list<Line> listOfTrackLines1 = pairOfLineLists.first;
	std::list<Line> listOfTrackLines2 = pairOfLineLists.second;

	//Check for intersections
	for (auto const & trackLine : listOfTrackLines1)
	{
		if (line.intersects(trackLine, placeholder))
		{
			intersections.push_back(placeholder);
		}
	}
	for (auto const & trackLine : listOfTrackLines2)
	{
		if (line.intersects(trackLine, placeholder))
		{
			intersections.push_back(placeholder);
		}
	}

	return intersections;
}




//////////////////////////////////////////////////////////////
//Calculate Position inside the Track that is near to position

sf::Vector2f Track::calculatePositionInTrackNear(sf::Vector2f const & position) const
{
	bool first = true;
	float smallestDist;
	sf::Vector2f bestPos;
	for (auto const & segment : mBorderTrack)
	{
		sf::Vector2f center = mySFML::Simple::meanVector(segment.first, segment.second);
		if (first)
		{
			first = false;
			bestPos = center;
			smallestDist = mySFML::Simple::lengthOf(position - center);
		}
		else
		{
			float newDist = mySFML::Simple::lengthOf(position - center);
			if (newDist < smallestDist)
			{
				smallestDist = newDist;
				bestPos = center;
			}
		}
	}
	return bestPos;
}




//////////////////////////////
//Save or Load to or from File

//Save to file
bool Track::saveToFile(std::string const & path) const
{
	std::ofstream fileStream(path, std::ios::trunc);
	if (fileStream.is_open())
	{
		std::cout << "Saving Track File: \"" << path << "\"...";

		fileStream << "<BorderTrackBase:Begin>" << std::endl;
		fileStream << mBorderTrack;
		fileStream << "<BorderTrackBase:End>" << std::endl;

		fileStream << "<ForwardDirection:Begin>" << std::endl;
		fileStream << mTrackDirectionIsForwards << std::endl;
		fileStream << "<ForwardDirection:End>" << std::endl;

		fileStream << "<StartPosition:Begin>" << std::endl;
		fileStream << mStartPosition.x << " " << mStartPosition.y << std::endl;
		fileStream << "<StartPosition:End>" << std::endl;

		fileStream.close();
		std::cout << "Complete!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Saving Track File...Failed to open File: \"" << path << "\"!" << std::endl;
		return false;
	}
}

//Load from File
bool Track::loadFromFile(std::string const & path)
{
	std::ifstream fileStream(path);
	if (fileStream.is_open())
	{
		std::cout << "Load Track File: \"" << path << "\"...";

		//Extract Lines
		std::list<std::string> listOfLines;
		std::string getLine;
		while (std::getline(fileStream, getLine))
		{
			listOfLines.push_back(getLine);
		}

		//Cut into different parts
		std::function<std::list<std::string>(std::list<std::string> const &, std::string const &)> extractPart = 
			[](std::list<std::string> const & listOfLines, std::string const & part) -> std::list<std::string>
		{
			std::list<std::string> listOfLinesInPart;
			bool foundPartBeginMarker = false;
			for (std::list<std::string>::const_iterator it = listOfLines.begin(); it != listOfLines.end(); ++it)
			{
				if (*it == "<" + part + ":End>")
				{
					break;
				}
				if (foundPartBeginMarker)
				{
					listOfLinesInPart.push_back(*it);
				}
				if (*it == "<" + part + ":Begin>")
				{
					foundPartBeginMarker = true;
				}
			}
			if (!foundPartBeginMarker)
			{
				std::cout << "Error! Part Name \"" + part + "\" was not found!" << std::endl;

				return std::list<std::string>();
			}
			return std::move(listOfLinesInPart);
		};

		std::list<std::string> listOfBorderTrackBaseLines = extractPart(listOfLines, "BorderTrackBase");
		std::list<std::string> listOfForwardDirectionLines = extractPart(listOfLines, "ForwardDirection");
		std::list<std::string> listOfStartPositionLines = extractPart(listOfLines, "StartPosition");

		if (listOfBorderTrackBaseLines.empty() || listOfForwardDirectionLines.size() != 1 || listOfStartPositionLines.size() != 1)
		{
			std::cout << "Error! Format incorrect! Failed to load File!" << std::endl;
			return false;
		}

		//Extract data from listOfBorderTrackBaseLines
		float x1, y1, x2, y2;
		std::list<BorderTrackSegment> listOfBorderTrackSegments;
		for (auto const & line : listOfBorderTrackBaseLines)
		{
			std::stringstream sstream(line);
			sstream >> x1 >> y1 >> x2 >> y2;
			listOfBorderTrackSegments.push_back(std::make_pair(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2)));
		}
		BorderTrackBase borderTrackBase(listOfBorderTrackSegments);

		//Extract data from listOfForwardDirectionLines
		bool forwardDirection = std::stoi(listOfForwardDirectionLines.front());
		
		//Extract data from listOfStartPositionLines
		float startX, startY;
		std::stringstream stream(listOfStartPositionLines.front());
		stream >> startX >> startY;
		sf::Vector2f startPosition(startX, startY);
		
		//Close File Stream
		fileStream.close();

		//Set State
		*this = Track(borderTrackBase, startPosition, forwardDirection);
		std::cout << "Complete!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Load Track File...Failed to open File: \"" << path << "\"!" << std::endl;
		return false;
	}
}




void Track::addSegmentNearestTo(sf::Vector2f const & pos)
{
	//Find nearest center
	BorderTrackBase::iterator nearestIt;
	float nearestDist;
	sf::Vector2f nearestCenter;
	bool first = true;
	for (BorderTrackBase::iterator it = mBorderTrack.begin(); it != mBorderTrack.end(); ++it)
	{
		sf::Vector2f center = (it->first + it->second) / 2.f;
		float dist = mySFML::Simple::lengthOf(pos - center);
		if (first)
		{
			nearestIt = it;
			nearestDist = dist;
			nearestCenter = center;
			first = false;
		}
		else
		{
			if (dist < nearestDist)
			{
				nearestIt = it;
				nearestDist = dist;
				nearestCenter = center;
			}
		}
	}

	//Find nearer neighbour
	BorderTrackBase::iterator nextIt = nearestIt;
	BorderTrackBase::iterator previousIt = nearestIt;
	++nextIt;
	if (nextIt == mBorderTrack.end())
	{
		nextIt = mBorderTrack.begin();
	}
	if (previousIt == mBorderTrack.begin())
	{
		previousIt = mBorderTrack.end();
	}
	--previousIt;
	sf::Vector2f nextCenter = (nextIt->first + nextIt->second) / 2.f;
	sf::Vector2f previousCenter = (previousIt->first + previousIt->second) / 2.f;
	float nextDist = mySFML::Simple::lengthOf(nextCenter - pos);
	float previousDist = mySFML::Simple::lengthOf(previousCenter - pos);
	bool nextIsNearer = (nextDist < previousDist);
	bool innerProductOfNextPositive = (mySFML::Simple::scalarProduct(nextCenter - nearestCenter, pos - nearestCenter) > 0.f);
	BorderTrackBase::iterator secondNearestIt = (innerProductOfNextPositive ? nextIt : previousIt );

	//Add segment
	BorderTrackSegment newSegment = std::make_pair(mySFML::Simple::meanVector(nearestIt->first, secondNearestIt->first), mySFML::Simple::meanVector(nearestIt->second, secondNearestIt->second));
	if (innerProductOfNextPositive)
	{
		mBorderTrack.addSegmentBefore(nextIt, newSegment);
	}
	else
	{
		mBorderTrack.addSegmentBefore(nearestIt, newSegment);
	}
}


//Returns an Iterator to the BorderTrackSegment which is in circle of radius radius around position via reference (Return value: true, if such Segment was found, int& type: 1, if point is centerPoint; 2, if point is borderPoint and in pair::first; 3, if point is borderPoint and in pair::second)
bool Track::getIteratorToBorderTrackSegmentWhichHasBorder(sf::Vector2f const & position, float radius, BorderTrackBase::iterator & iterator, int& type)
{
	//Determine all possible candidates
	std::list<std::pair<BorderTrackBase::iterator, std::pair<int, float>>> listOfFunctioningIteratorFirstAndDistancePairs;
	for (BorderTrackBase::iterator it = mBorderTrack.begin(); it != mBorderTrack.end(); ++it)
	{
		float firstDist = mySFML::Simple::lengthOf(it->first - position);
		if (firstDist < radius)
		{
			listOfFunctioningIteratorFirstAndDistancePairs.push_back(std::make_pair(it, std::make_pair(2, firstDist)));
		}
		float secondDist = mySFML::Simple::lengthOf(it->second - position);
		if (secondDist < radius)
		{
			listOfFunctioningIteratorFirstAndDistancePairs.push_back(std::make_pair(it, std::make_pair(3, secondDist)));
		}
		float midDist = mySFML::Simple::lengthOf(mySFML::Simple::meanVector(it->first, it->second) - position);
		if (midDist < radius)
		{
			listOfFunctioningIteratorFirstAndDistancePairs.push_back(std::make_pair(it, std::make_pair(1, midDist)));
		}
	}

	//Handle special cases
	if (listOfFunctioningIteratorFirstAndDistancePairs.empty())
	{
		return false;
	}

	//Filter out the best candidate
	BorderTrackBase::iterator bestIt;
	int bestType;
	float bestDistance;
	bool firstLoop = true;
	for (auto const & pair : listOfFunctioningIteratorFirstAndDistancePairs)
	{
		if (firstLoop)
		{
			firstLoop = false;
			bestIt = pair.first;
			bestType = pair.second.first;
			bestDistance = pair.second.second;
		}
		else
		{
			if (pair.second.second < bestDistance)
			{
				bestIt = pair.first;
				bestType = pair.second.first;
				bestDistance = pair.second.second;
			}
		}
	}

	//Return per Return value and Reference
	iterator = bestIt;
	type = bestType;
	return true;
}



//////////////////////////
//Private intern functions

//Refresh VertexArray
void Track::refreshState()
{
	mVertexArrayOfTrack.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	mVertexArrayOfTrack.clear();
	for (auto const & trackSegment : mBorderTrack.getListOfBorderTrackSegments())
	{
		mVertexArrayOfTrack.append(sf::Vertex(trackSegment.first, mTrackColor));
		mVertexArrayOfTrack.append(sf::Vertex(trackSegment.second, mTrackColor));
	}
	if (!mBorderTrack.getListOfBorderTrackSegments().empty())
	{
		mVertexArrayOfTrack.append(sf::Vertex(mBorderTrack.getListOfBorderTrackSegments().front().first, mTrackColor));
		mVertexArrayOfTrack.append(sf::Vertex(mBorderTrack.getListOfBorderTrackSegments().front().second, mTrackColor));
	}
}



/////////////////////////
//Public static functions

//Construct Circle Track
Track Track::constructCircleTrack(sf::Vector2f const & center, float radius, unsigned int pointCount, float width)
{
	std::cout << "Construct Circle Track...";
	std::list<CenterTrackSegment> list;
	for (unsigned int i = 0; i < pointCount; ++i)
	{
		float angle = static_cast<float>(i) / pointCount * 2.f * myMath::Const::PIf;
		sf::Vector2f position = radius * mySFML::Create::createNormalVectorWithAngle(angle) + center;
		sf::Vector2f relVec = width * mySFML::Create::createNormalVectorWithAngle(angle);
		list.push_back(std::make_pair(position, relVec));
	}

	std::cout << "Finished!" << std::endl;
	return Track(CenterTrackBase(list), list.front().first, true);
}
