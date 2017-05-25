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

