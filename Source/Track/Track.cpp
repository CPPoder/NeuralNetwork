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
	this->loadFromFile(filePath);
}

//Constructor from BorderTrackBase
Track::Track(BorderTrackBase const & borderTrackBase, sf::FloatRect const & validTrackArea, sf::Color const & color = sf::Color::White)
{
	this->setTrack(borderTrackBase, validTrackArea);
	this->setColor(color);
}

//Constructor from CenterTrackBase
Track::Track(CenterTrackBase const & centerTrackBase, sf::FloatRect const & validTrackArea, sf::Color const & color = sf::Color::White)
{
	this->setTrack(centerTrackBase, validTrackArea);
	this->setColor(color);
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

void Track::setTrack(BorderTrackBase const & borderTrackBase, sf::FloatRect const & validTrackArea)
{
	mBorderTrack = borderTrackBase;
	mValidTrackArea = validTrackArea;
	this->refreshState();
}

void Track::setTrack(CenterTrackBase const & centerTrackBase, sf::FloatRect const & validTrackArea)
{
	mBorderTrack = centerTrackBase.getBorderTrackBase();
	mValidTrackArea = validTrackArea;
	this->refreshState();
}

void Track::setColor(sf::Color const & color)
{
	mTrackColor = color;
	this->refreshState();
}

void Track::setValidTrackArea(sf::FloatRect const & floatRectOfValidTrackArea)
{
	mValidTrackArea = floatRectOfValidTrackArea;
}



////////
//Getter

sf::Color Track::getColor() const
{
	return mTrackColor;
}

sf::FloatRect Track::getValidTrackArea() const
{
	return mValidTrackArea;
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
	std::function<std::pair<Line, Line>(CenterTrackSegment const &, CenterTrackSegment const &)> linePairGetter = 
		[](CenterTrackSegment const & segment1, CenterTrackSegment const & segment2) -> std::pair<Line, Line>
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




//////////////////////////////////////////////////////////////
//Calculate Position inside the Track that is near to position

sf::Vector2f Track::calculatePositionInTrackNear(sf::Vector2f const & position) const
{
	sf::Vector2f nearestPosition;
	float dist;
	bool firstSegment = true;
	for (auto const & segment : mBorderTrack.getCenterTrackBase().getListOfCenterTrackSegments())
	{
		sf::Vector2f newPos = segment.first;
		float newDist = mySFML::Simple::lengthOf(segment.second);
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

		fileStream << "<BorderTrackBase:Begin>" << std::endl;
		fileStream << mBorderTrack;
		fileStream << "<BorderTrackBase:End>" << std::endl;

		fileStream << "<ValidTrackArea:Begin>" << std::endl;
		fileStream << mValidTrackArea.left << '\t' << mValidTrackArea.top << '\t' << mValidTrackArea.width << '\t' << mValidTrackArea.height << std::endl;
		fileStream << "<ValidTrackArea:End>" << std::endl;

		fileStream.close();
		std::cout << "Complete!" << std::endl;
	}
	else
	{
		std::cout << "Saving Track File...Failed to open File: \"" << path << "\"!" << std::endl;
	}
}

//Load from File
void Track::loadFromFile(std::string const & path)
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
				if (foundPartBeginMarker)
				{
					listOfLinesInPart.push_back(*it);
				}
				if (*it == "<" + part + ":Begin>")
				{
					foundPartBeginMarker = true;
				}
				if (*it == "<" + part + ":End>")
				{
					break;
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
		std::list<std::string> listOfValidTrackAreaLines = extractPart(listOfLines, "ValidTrackArea");
		if (listOfBorderTrackBaseLines.empty() || listOfValidTrackAreaLines.empty())
		{
			std::cout << "Error! Format incorrect! Failed to load File!" << std::endl;
			return;
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

		//Extract data from listOfValidTrackAreaLines
		float x, y, w, h;
		std::string validTrackAreaString;
		if (listOfValidTrackAreaLines.size() == 1u)
		{
			validTrackAreaString = listOfValidTrackAreaLines.front();
		}
		else
		{
			std::cout << "Error! Format incorrect! Failed to load File!" << std::endl;
			return;
		}
		std::stringstream sstream(validTrackAreaString);
		sstream >> x >> y >> w >> h;
		sf::FloatRect validArea(x, y, w, h);
		
		//Close File Stream
		fileStream.close();

		//Set State
		this->setTrack(borderTrackBase, validArea);
		std::cout << "Complete!" << std::endl;
	}
	else
	{
		std::cout << "Load Track File...Failed to open File: \"" << path << "\"!" << std::endl;
	}
}



//Deform Track
void Track::deformRandomly(unsigned int numberOfDeformations, sf::Vector2f const & sizeOfValidTrackArea, float deformationStep)
{
	std::cout << "Start Deformation of Track! (Number of Deformations: " << numberOfDeformations << ")" << std::endl;
	std::list<CenterWidthTrackSegment> originalTrack(std::move(this->getListOfCenterWidthTrackSegments()));
	std::cout << "Debugging Validity Check 0: " << Track(originalTrack).checkIfTrackIsValid(sizeOfValidTrackArea) << std::endl;
	std::list<CenterWidthTrackSegment> deformedTrack;
	int progressStep = 0;
	int fails = 0;
	for (unsigned int i = 0; i < numberOfDeformations; ++i)
	{
		if (static_cast<float>(i) / static_cast<float>(numberOfDeformations) * 100.f > progressStep)
		{
			progressStep += 1;
			std::cout << "Create Track! Progress: " << progressStep << " %" << std::endl;
		}
		deformedTrack = std::move(Track::doOneRandomDeformation(originalTrack, deformationStep));
		if (Track(deformedTrack).checkIfTrackIsValid(sizeOfValidTrackArea))
		{
			originalTrack = deformedTrack;
		}
		else
		{
			deformedTrack = originalTrack;
			++fails;
		}
	}
	//std::cout << "Debugging Validity Check 1: " << this->checkIfTrackIsValid(sizeOfValidTrackArea) << std::endl;
	this->setTrack(Track::convertIntoListOfBorderTrackSegments(deformedTrack));
	std::cout << "Deformation of Track finished! (" << fails << " fails of " << numberOfDeformations << ", Number of Segments: " << mListOfTrackSegments.size() << ")" << std::endl;
	//std::cout << "Debugging Validity Check 2: " << this->checkIfTrackIsValid(sizeOfValidTrackArea) << std::endl;
}

//Double the Number of Segments
void Track::doubleNumberOfSegments()
{
	unsigned int size = mListOfTrackSegments.size();
	std::cout << "Double the number of Segments from " << size << " to "  << 2 * size << "...";

	std::list<BorderTrackSegment> newList;
	std::list<BorderTrackSegment>::const_iterator it1 = mListOfTrackSegments.begin();
	std::list<BorderTrackSegment>::const_iterator it2 = mListOfTrackSegments.begin();
	++it2;
	while (it2 != mListOfTrackSegments.end())
	{
		sf::Vector2f newPosL = mySFML::Simple::meanVector(it1->first, it2->first);
		sf::Vector2f newPosR = mySFML::Simple::meanVector(it1->second, it2->second);
		newList.push_back(*it1);
		newList.push_back(std::make_pair(newPosL, newPosR));
		++it1;
		++it2;
	}
	sf::Vector2f newPosL = mySFML::Simple::meanVector(mListOfTrackSegments.back().first, mListOfTrackSegments.front().first);
	sf::Vector2f newPosR = mySFML::Simple::meanVector(mListOfTrackSegments.back().second, mListOfTrackSegments.front().second);
	newList.push_back(mListOfTrackSegments.back());
	newList.push_back(std::make_pair(newPosL, newPosR));

	this->setTrack(newList);

	std::cout << "Finished!" << std::endl;
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
			//std::cout << "Fail 1!" << std::endl;
			return false;
		}
		if (!(mySFML::Comp::smaller(sf::Vector2f(0.f, 0.f), line.vertex2) && mySFML::Comp::smaller(line.vertex2, sizeOfValidTrackArea)))
		{
			//std::cout << "Fail 2!" << std::endl;
			return false;
		}
	}
	for (auto const & line : listOfLines2)
	{
		if (!(mySFML::Comp::smaller(sf::Vector2f(0.f, 0.f), line.vertex1) && mySFML::Comp::smaller(line.vertex1, sizeOfValidTrackArea)))
		{
			//std::cout << "Fail 3!" << std::endl;
			return false;
		}
		if (!(mySFML::Comp::smaller(sf::Vector2f(0.f, 0.f), line.vertex2) && mySFML::Comp::smaller(line.vertex2, sizeOfValidTrackArea)))
		{
			//std::cout << "Fail 4!" << std::endl;
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
				//std::cout << "Fail 5!" << std::endl;
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
		//std::cout << "Fail 6!" << std::endl;
		return false;
	}

	//Thirdly: list2 and list2
	if (checkForIntersectionInListOfLines(listOfLines2))
	{
		//std::cout << "Fail 7!" << std::endl;
		return false;
	}

	
	//Check here maybe for direction conservation (Inner product of neighbouring lines should be greater than 0 or even more)
	std::function<bool(std::list<Line> const &)> checkForDirectionConservation =
		[](std::list<Line> const & listOfLines) -> bool
	{
		float constexpr scalarProductLimit = 0.5f;

		std::list<Line>::const_iterator lineIt1 = listOfLines.begin();
		std::list<Line>::const_iterator lineIt2 = listOfLines.begin();
		++lineIt2;
		while (lineIt2 != listOfLines.end())
		{
			sf::Vector2f vec1 = lineIt1->getUnitTangentVector();
			sf::Vector2f vec2 = lineIt2->getUnitTangentVector();
			if (mySFML::Simple::scalarProduct(vec1, vec2) < scalarProductLimit)
			{
				//std::cout << "Haltepunkt!" << std::endl;
				return false;
			}
			++lineIt1;
			++lineIt2;
		}
		sf::Vector2f vec1 = lineIt1->getUnitTangentVector();
		sf::Vector2f vec2 = listOfLines.front().getUnitTangentVector();
		if (mySFML::Simple::scalarProduct(vec1, vec2) < scalarProductLimit)
		{
			return false;
		}
		return true;
	};

	if (!checkForDirectionConservation(listOfLines1))
	{
		//std::cout << "Fail 8!" << std::endl;
		return false;
	}
	if (!checkForDirectionConservation(listOfLines2))
	{
		//std::cout << "Fail 9!" << std::endl;
		return false;
	}

	//At the end, return true in the no-intersections case
	//std::cout << "No Fail!" << std::endl;
	return true;
}



/////////////////////////
//Public static functions

//Construct Circle Track
std::list<CenterWidthTrackSegment> Track::constructCircleTrack(sf::Vector2f const & center, float radius, unsigned int pointCount, float width)
{
	std::cout << "Construct Circle Track...";
	std::list<CenterWidthTrackSegment> list;
	for (unsigned int i = 0; i < pointCount; ++i)
	{
		list.push_back(std::make_pair(radius * mySFML::Create::createNormalVectorWithAngle(static_cast<float>(i) / (pointCount + 1u) * 2.f * myMath::Const::PIf) + center, width));
	}

	std::cout << "Finished!" << std::endl;
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
	track.deformRandomly(numberOfDeformations, sizeOfValidTrackArea, 0.1f);
	return track.getListOfCenterWidthTrackSegments();
}