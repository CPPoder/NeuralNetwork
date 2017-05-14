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

//Default Constructor
Track::Track()
	: Track(std::list<BorderTrackSegment>())
{
}

//Constructor from File
Track::Track(std::string const & filePath)
	: Track()
{
	this->loadFromFile(filePath);
}

//Constructor for TrackSegment Lists
Track::Track(std::list<BorderTrackSegment> const & listOfTrackSegments, sf::Color const & color)
	: mTrackColor(color)
{
	this->setTrack(listOfTrackSegments);
}

//Constructor for TrackMiddles and Widths Lists
Track::Track(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths, sf::Color const & color)
	: Track(std::move(this->convertIntoListOfBorderTrackSegments(listOfPositionsAndWidths)), color)
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
void Track::setTrack(std::list<BorderTrackSegment> const & listOfTrackSegments)
{
	mListOfTrackSegments = listOfTrackSegments;
	//std::cout << "Check for validity of the track: " << this->checkIfTrackIsValid() << std::endl;
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

//Get Lists of Lines
std::pair<std::list<Line>, std::list<Line>> Track::getListsOfLines() const
{
	std::list<Line> listOfLines1;
	std::list<Line> listOfLines2;
	std::list<BorderTrackSegment>::const_iterator trackSegmentIt1 = mListOfTrackSegments.begin();
	std::list<BorderTrackSegment>::const_iterator trackSegmentIt2 = ++mListOfTrackSegments.begin();
	while (trackSegmentIt2 != mListOfTrackSegments.end())
	{
		listOfLines1.push_back(Line(trackSegmentIt1->first, trackSegmentIt2->first));
		listOfLines2.push_back(Line(trackSegmentIt1->second, trackSegmentIt2->second));
		++trackSegmentIt1;
		++trackSegmentIt2;
	}
	listOfLines1.push_back(Line(trackSegmentIt1->first, mListOfTrackSegments.front().first));
	listOfLines2.push_back(Line(trackSegmentIt1->second, mListOfTrackSegments.front().second));
	return std::move(std::make_pair(std::move(listOfLines1), std::move(listOfLines2)));
}

//Get List of CenterWidthTrackSegments
std::list<CenterWidthTrackSegment> Track::getListOfCenterWidthTrackSegments() const
{
	std::list<CenterWidthTrackSegment> list;
	for (auto segment : mListOfTrackSegments)
	{
		list.push_back(std::make_pair(mySFML::Simple::meanVector(segment.first, segment.second), mySFML::Simple::lengthOf(segment.second - segment.first) / 2.f));
	}
	return std::move(list);
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
	std::list<BorderTrackSegment>::const_iterator trackSegIt1 = mListOfTrackSegments.begin();
	std::list<BorderTrackSegment>::const_iterator trackSegIt2 = ++mListOfTrackSegments.begin();
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

//////////////////////////////////////////////////////////////
//Calculate Position inside the Track that is near to position

sf::Vector2f Track::calculatePositionInTrackNear(sf::Vector2f const & position) const
{
	sf::Vector2f nearestPosition;
	float dist;
	bool firstSegment = true;
	for (auto const & segment : mListOfTrackSegments)
	{
		sf::Vector2f newPos = mySFML::Simple::meanVector(segment.first, segment.second);
		float newDist = mySFML::Simple::lengthOf(newPos - position);
		if (firstSegment)
		{
			firstSegment = false;
			nearestPosition = newPos;
			dist = newDist;
		}
		else if (newDist < dist)
		{
			nearestPosition = newPos;
			dist = newDist;
		}
	}
	return nearestPosition;
}

//////////////////////////////
//Save or Load to or from File

//Save to file
void Track::saveToFile(std::string const & path) const
{
	std::ofstream fileStream(path, std::ios::trunc);
	if (fileStream.is_open())
	{
		std::cout << "Saving Track File: \"" << path << "\"...";

		bool firstLine = true;
		for (auto segment : mListOfTrackSegments)
		{
			if (!firstLine)
			{
				fileStream << std::endl;
			}
			firstLine = false;
			fileStream << segment.first.x << "\t" << segment.first.y << "\t" << segment.second.x << "\t" << segment.second.y;
		}

		fileStream.close();
		std::cout << "Complete!" << std::endl;
	}
	else
	{
		std::cout << "Saving Track File...Failed to open File: " << path << std::endl;
	}
}

//Load from File
void Track::loadFromFile(std::string const & path)
{
	std::ifstream fileStream(path);
	if (fileStream.is_open())
	{
		std::cout << "Load Track File: \"" << path << "\"...";

		std::list<std::string> listOfLines;
		std::string getLine;
		while (std::getline(fileStream, getLine))
		{
			listOfLines.push_back(getLine);
		}
		float x1, y1, x2, y2;
		std::list<BorderTrackSegment> listOfBorderTrackSegments;
		for (auto const & line : listOfLines)
		{
			std::stringstream sstream(line);
			sstream >> x1 >> y1 >> x2 >> y2;
			listOfBorderTrackSegments.push_back(std::make_pair(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2)));
		}
		
		fileStream.close();

		this->setTrack(listOfBorderTrackSegments);
		std::cout << "Complete!" << std::endl;
	}
	else
	{
		std::cout << "Load Track File...Failed to open File: " << path << std::endl;
	}
}



//Deform Track
void Track::deformRandomly(unsigned int numberOfDeformations, sf::Vector2f const & sizeOfValidTrackArea)
{
	std::cout << "Start Deformation of Track! (Number of Deformations: " << numberOfDeformations << ")" << std::endl;
	std::list<CenterWidthTrackSegment> originalTrack(std::move(this->getListOfCenterWidthTrackSegments()));
	std::list<CenterWidthTrackSegment> deformedTrack;
	int progressStep = 0;
	for (unsigned int i = 0; i < numberOfDeformations; ++i)
	{
		if (static_cast<float>(i) / static_cast<float>(numberOfDeformations) * 100.f > progressStep)
		{
			progressStep += 1;
			std::cout << "Create Track! Progress: " << progressStep << " %" << std::endl;
		}
		deformedTrack = std::move(Track::doOneRandomDeformation(originalTrack, 5.f));
		if (Track(deformedTrack).checkIfTrackIsValid(sizeOfValidTrackArea))
		{
			originalTrack = deformedTrack;
		}
		else
		{
			deformedTrack = originalTrack;
		}
	}
	this->setTrack(Track::convertIntoListOfBorderTrackSegments(deformedTrack));
	std::cout << "Deformation of Track finished!" << std::endl;
}



//////////////////////////
//Private intern functions

//Refresh VertexArray
void Track::refreshVertexArray()
{
	mVertexArrayOfTrack.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	mVertexArrayOfTrack.clear();
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


bool Track::checkIfTrackIsValid(sf::Vector2f const & sizeOfValidTrackArea) const
{
	//Get Lists Of Lines
	auto listsOfLines = std::move(this->getListsOfLines());
	std::list<Line> listOfLines1 = std::move(listsOfLines.first);
	std::list<Line> listOfLines2 = std::move(listsOfLines.second);

	//Check if Track is inside its frame
	for (auto const & line : listOfLines1)
	{
		if (!(mySFML::Comp::smaller(sf::Vector2f(0.f, 0.f), line.vertex1) && mySFML::Comp::smaller(line.vertex1, sizeOfValidTrackArea)))
		{
			return false;
		}
		if (!(mySFML::Comp::smaller(sf::Vector2f(0.f, 0.f), line.vertex2) && mySFML::Comp::smaller(line.vertex2, sizeOfValidTrackArea)))
		{
			return false;
		}
	}
	for (auto const & line : listOfLines2)
	{
		if (!(mySFML::Comp::smaller(sf::Vector2f(0.f, 0.f), line.vertex1) && mySFML::Comp::smaller(line.vertex1, sizeOfValidTrackArea)))
		{
			return false;
		}
		if (!(mySFML::Comp::smaller(sf::Vector2f(0.f, 0.f), line.vertex2) && mySFML::Comp::smaller(line.vertex2, sizeOfValidTrackArea)))
		{
			return false;
		}
	}
	

	//Check for overlap between any but not neighboured Lines
	//At first: list1 and list2
	for (auto const & line1 : listOfLines1)
	{
		for (auto const & line2 : listOfLines2)
		{
			if (line1.intersects(line2))
			{
				return false;
			}
		}
	}

	//Create Lambda for usage with both lists
	std::function<bool(std::list<Line> const &)> checkForIntersectionInListOfLines = 
		[](std::list<Line> const & listOfLines) -> bool
	{
		for (std::list<Line>::const_iterator lineIt1 = listOfLines.begin(); lineIt1 != listOfLines.end(); ++lineIt1)
		{
			std::list<Line>::const_iterator lineIt2 = lineIt1;
			if (++lineIt2 == listOfLines.end())
			{
				break;
			}
			//Now check intersection with neighbour
			sf::Vector2f intersectionPoint(0.f, 0.f);
			if (lineIt1->intersects(*lineIt2, intersectionPoint))
			{
				sf::Vector2f sharedPoint(lineIt1->vertex2);
				if (mySFML::Simple::lengthOf(sharedPoint - intersectionPoint) > 1.0E-05f)
				{
					return true;
				}
			}
			while (++lineIt2 != listOfLines.end())
			{
				auto checkIt = lineIt2;
				if (++checkIt == listOfLines.end())
				{
					if (lineIt1 == listOfLines.begin())
					{
						break; //Exclude case in which first line checks with last line
					}
				}
				//Check other intersections
				if (lineIt1->intersects(*lineIt2))
				{
					return true;
				}
			}
		}
		return false;
	};

	//Secondly: list1 and list1
	if (checkForIntersectionInListOfLines(listOfLines1))
	{
		return false;
	}

	//Thirdly: list2 and list2
	if (checkForIntersectionInListOfLines(listOfLines2))
	{
		return false;
	}
	
	//Check here maybe for direction conservation (Inner product of neighbouring lines should be greater than 0 or even more)
	std::function<bool(std::list<Line> const &)> checkForDirectionConservation =
		[](std::list<Line> const & listOfLines) -> bool
	{
		std::list<Line>::const_iterator lineIt1 = listOfLines.begin();
		std::list<Line>::const_iterator lineIt2 = listOfLines.begin();
		++lineIt2;
		while (lineIt2 != listOfLines.end())
		{
			sf::Vector2f vec1 = lineIt1->getUnitTangentVector();
			sf::Vector2f vec2 = lineIt2->getUnitTangentVector();
			if (mySFML::Simple::scalarProduct(vec1, vec2) < 0.5f)
			{
				return false;
			}
			++lineIt1;
			++lineIt2;
		}
		sf::Vector2f vec1 = lineIt1->getUnitTangentVector();
		sf::Vector2f vec2 = listOfLines.front().getUnitTangentVector();
		if (mySFML::Simple::scalarProduct(vec1, vec2) < 0.f)
		{
			return false;
		}
		return true;
	};

	if (!checkForDirectionConservation(listOfLines1))
	{
		return false;
	}
	if (!checkForDirectionConservation(listOfLines2))
	{
		return false;
	}


	//At the end, return true in the no-intersections case
	return true;
}


/////////////////////////////////
//Private static intern functions

//Convert List of Positions and Widths into List of Track Segments
std::list<BorderTrackSegment> Track::convertIntoListOfBorderTrackSegments(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths)
{
	//Exclude pathological cases
	if (listOfPositionsAndWidths.empty())
	{
		return std::list<BorderTrackSegment>();
	}

	//Setup
	std::list<BorderTrackSegment> listOfTrackSegments;
	std::list<CenterWidthTrackSegment>::const_iterator pairIt = listOfPositionsAndWidths.begin();
	std::list<CenterWidthTrackSegment>::const_iterator nextIt = pairIt;
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


/////////////////////////
//Public static functions

//Construct Circle Track
std::list<CenterWidthTrackSegment> Track::constructCircleTrack(sf::Vector2f const & center, float radius, unsigned int pointCount, float width)
{
	std::list<CenterWidthTrackSegment> list;
	for (unsigned int i = 0; i < pointCount; ++i)
	{
		list.push_back(std::make_pair(radius * mySFML::Create::createNormalVectorWithAngle(static_cast<float>(i) / (pointCount + 1u) * 2.f * myMath::Const::PIf) + center, width));
	}

	return std::move(list);
}

//Randomly deform Track
std::list<CenterWidthTrackSegment> Track::doOneRandomDeformation(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths, float maximalDeformationLength)
{
	std::list<CenterWidthTrackSegment> deformedTrack(listOfPositionsAndWidths);
	unsigned int size = deformedTrack.size();
	unsigned int randomNumber = myMath::Rand::randIntervali(0, size - 1);
	sf::Vector2f randomDeformation = sf::Vector2f(myMath::Rand::randIntervalf(-100, 100) / 100.f * maximalDeformationLength, myMath::Rand::randIntervalf(-100, 100) / 100.f * maximalDeformationLength);
	std::list<CenterWidthTrackSegment>::iterator it = deformedTrack.begin();
	std::advance(it, randomNumber);
	it->first += randomDeformation;
	return std::move(deformedTrack);
}


//Create Random Track
std::list<CenterWidthTrackSegment> Track::createRandomTrack(unsigned int numberOfDeformations, sf::Vector2f const & sizeOfValidTrackArea)
{
	Track track(Track::constructCircleTrack(sf::Vector2f(50.f, 50.f), 40.f, 50u, 6.f));
	track.deformRandomly(numberOfDeformations, sizeOfValidTrackArea);
	return track.getListOfCenterWidthTrackSegments();
}