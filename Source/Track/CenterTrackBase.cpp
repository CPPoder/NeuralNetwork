#include "stdafx.h"
#include "Source\Track\CenterTrackBase.hpp"
#include "Source\Track\BorderTrackBase.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"


CenterTrackBase::CenterTrackBase(std::list<CenterTrackSegment> const & listOfCenterTrackSegments)
	: mListOfCenterTrackSegments(listOfCenterTrackSegments)
{
}




CenterTrackBase& CenterTrackBase::doForAllSectors(std::function<void(CenterTrackSegment &, CenterTrackSegment &)> task)
{
	if (!mListOfCenterTrackSegments.empty())
	{
		std::list<CenterTrackSegment>::iterator itOne = mListOfCenterTrackSegments.begin();
		std::list<CenterTrackSegment>::iterator itTwo = itOne;
		++itTwo;

		while (itTwo != mListOfCenterTrackSegments.end())
		{
			task(*itOne, *itTwo);
			++itOne;
			++itTwo;
		}
		task(mListOfCenterTrackSegments.back(), mListOfCenterTrackSegments.front());
	}
	return *this;
}



BorderTrackBase CenterTrackBase::getBorderTrackBase() const
{
	std::function<BorderTrackSegment(CenterTrackSegment const &, CenterTrackSegment const &)> trafoCenterToBorderSegments =
		[](CenterTrackSegment const & segment1, CenterTrackSegment const & segment2) -> BorderTrackSegment
	{
		sf::Vector2f point1 = segment1.first + segment1.second;
		sf::Vector2f point2 = segment1.first - segment1.second;
		return std::make_pair(point1, point2);
	};

	std::list<BorderTrackSegment> listOfBorderTrackSegments(this->calculateForAllSectorsAndPutIntoList(trafoCenterToBorderSegments));
	return BorderTrackBase(listOfBorderTrackSegments);
}


std::list<CenterTrackSegment> const & CenterTrackBase::getListOfCenterTrackSegments() const
{
	return mListOfCenterTrackSegments;
}
void CenterTrackBase::setListOfCenterTrackSegments(std::list<CenterTrackSegment> const & listOfCenterTrackSegments)
{
	mListOfCenterTrackSegments = listOfCenterTrackSegments;
}

void CenterTrackBase::setRelativeVectorsOrthogonal()
{
	std::function<std::list<CenterTrackSegment>::iterator(std::list<CenterTrackSegment>::iterator)> getNextIterator =
		[this](std::list<CenterTrackSegment>::iterator it) -> std::list<CenterTrackSegment>::iterator
	{
		if (++it == mListOfCenterTrackSegments.end())
		{
			it = mListOfCenterTrackSegments.begin();
		}
		return it;
	};

	std::function<std::list<CenterTrackSegment>::iterator(std::list<CenterTrackSegment>::iterator)> getPreviousIterator =
		[this](std::list<CenterTrackSegment>::iterator it) -> std::list<CenterTrackSegment>::iterator
	{
		if (it == mListOfCenterTrackSegments.begin())
		{
			it = mListOfCenterTrackSegments.end();
			--it;
		}
		else
		{
			--it;
		}
		return it;
	};

	for (std::list<CenterTrackSegment>::iterator centerIt = mListOfCenterTrackSegments.begin(); centerIt != mListOfCenterTrackSegments.end(); ++centerIt)
	{
		auto prevIt = getPreviousIterator(centerIt);
		auto nextIt = getNextIterator(centerIt);
		sf::Vector2f tangent = mySFML::Simple::normalize(nextIt->first - prevIt->first);
		sf::Vector2f normal = mySFML::Create::createOrthogonalVector(tangent);
		float directionFactor = ((mySFML::Simple::scalarProduct(normal, centerIt->second) > 0) ? 1.f : -1.f);
		normal *= directionFactor;
		centerIt->second = mySFML::Simple::lengthOf(centerIt->second) * normal;
		
		//Old idea! May be better or worse!
		//float meanRadius = myMath::Simple::meanValue(mySFML::Simple::lengthOf(prevIt->second), mySFML::Simple::lengthOf(nextIt->second));
		//centerIt->second = meanRadius * mySFML::Simple::normalize(mySFML::Simple::meanVector(prevIt->second, nextIt->second));
	}

}

