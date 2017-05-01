#include "stdafx.h"
#include "Source\Track\Track.hpp"

#include "Source\Car\Car.hpp"
#include "Source\Collision\CollisionArea.hpp"

#include <array>


//////////////
//Constructors

//Default Constructor
Track::Track()
	: Track(std::list<TrackSegment>())
{
}

//Constructor for TrackSegment Lists
Track::Track(std::list<TrackSegment> const & listOfTrackSegments, sf::Color const & color)
	: mTrackColor(color)
{
	this->setTrack(listOfTrackSegments);
}

//Construtor for TrackMiddles and Widths Lists
Track::Track(std::list<std::pair<sf::Vector2f, float>> const & listOfPositionsAndWidths, sf::Color const & color)
	: Track(std::move(this->convertIntoListOfTrackSegments(listOfPositionsAndWidths)), color)
{
}


////////
//Render

//Render
void Track::render(sf::RenderWindow* renderWindow)
{
	renderWindow->draw(mVertexArrayOfTrack);
}


////////
//Setter

//Set Track
void Track::setTrack(std::list<TrackSegment> const & listOfTrackSegments)
{
	mListOfTrackSegments = listOfTrackSegments;
	this->refreshVertexArray();
}

//Set Color
void Track::setColor(sf::Color const & color)
{
	mTrackColor = color;
	this->refreshVertexArray();
}


////////
//Getter

//Get Color
sf::Color Track::getColor() const
{
	return mTrackColor;
}

////////////////////
//Collision Checking

bool Track::checkCollisionWith(Car const & car) const
{
	sf::VertexArray const & carVertexArray = car.getVertexArrayReference();
	std::array<Line, 4u> arrayOfCarLines{
		Line(carVertexArray[0].position, carVertexArray[1].position),
		Line(carVertexArray[1].position, carVertexArray[2].position),
		Line(carVertexArray[2].position, carVertexArray[3].position),
		Line(carVertexArray[3].position, carVertexArray[0].position)
	};
	std::list<TrackSegment>::const_iterator trackSegIt1 = mListOfTrackSegments.begin();
	std::list<TrackSegment>::const_iterator trackSegIt2 = ++mListOfTrackSegments.begin();
	while (trackSegIt2 != mListOfTrackSegments.end())
	{
		Line line1(trackSegIt1->first, trackSegIt2->first);
		Line line2(trackSegIt1->second, trackSegIt2->second);
		for (auto const & line : arrayOfCarLines)
		{
			if (line1.intersects(line))
			{
				return true;
			}
			if (line2.intersects(line))
			{
				return true;
			}
		}
		++trackSegIt1;
		++trackSegIt2;
	}
	Line line1(trackSegIt1->first, mListOfTrackSegments.begin()->first);
	Line line2(trackSegIt1->second, mListOfTrackSegments.begin()->second);
	for (auto const & line : arrayOfCarLines)
	{
		if (line1.intersects(line))
		{
			return true;
		}
		if (line2.intersects(line))
		{
			return true;
		}
	}
	return false;
}


//////////////////////////
//Private intern functions

//Refresh VertexArray
void Track::refreshVertexArray()
{
	mVertexArrayOfTrack.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	for (auto trackSegment : mListOfTrackSegments)
	{
		mVertexArrayOfTrack.append(sf::Vertex(trackSegment.first, mTrackColor));
		mVertexArrayOfTrack.append(sf::Vertex(trackSegment.second, mTrackColor));
	}
	if (!mListOfTrackSegments.empty())
	{
		mVertexArrayOfTrack.append(sf::Vertex(mListOfTrackSegments.front().first, mTrackColor));
		mVertexArrayOfTrack.append(sf::Vertex(mListOfTrackSegments.front().second, mTrackColor));
	}
}


//////////////////////////
//Private static intern functions

//Convert List of Positions and Widths into List of Track Segments
std::list<TrackSegment> Track::convertIntoListOfTrackSegments(std::list<std::pair<sf::Vector2f, float>> const & listOfPositionsAndWidths)
{
	//Exclude pathological cases
	if (listOfPositionsAndWidths.empty())
	{
		return std::list<TrackSegment>();
	}

	//Setup
	std::list<TrackSegment> listOfTrackSegments;
	std::list<std::pair<sf::Vector2f, float>>::const_iterator pairIt = listOfPositionsAndWidths.begin();
	std::list<std::pair<sf::Vector2f, float>>::const_iterator nextIt = pairIt;
	++nextIt;

	//Construct TrackSegments
	while (nextIt != listOfPositionsAndWidths.end())
	{
		sf::Vector2f pos1 = pairIt->first;
		sf::Vector2f pos2 = nextIt->first;
		float width = pairIt->second;
		sf::Vector2f relVec = pos2 - pos1;
		sf::Vector2f normalVec = mySFML::Simple::normalize(mySFML::Create::createOrthogonalVector(relVec));
		listOfTrackSegments.push_back(std::make_pair(pos1 + width * normalVec, pos1 - width * normalVec));
		++pairIt;
		++nextIt;
	}

	//Construct last TrackSegment
	sf::Vector2f pos1 = pairIt->first;
	sf::Vector2f pos2 = listOfPositionsAndWidths.front().first;
	float width = pairIt->second;
	sf::Vector2f relVec = pos2 - pos1;
	sf::Vector2f normalVec = mySFML::Simple::normalize(mySFML::Create::createOrthogonalVector(relVec));
	listOfTrackSegments.push_back(std::make_pair(pos1 + width * normalVec, pos1 - width * normalVec));

	//Return listOfTrackSegments
	return listOfTrackSegments;
}

