#ifndef BORDERTRACKBASE_HPP
#define BORDERTRACKBASE_HPP


#include "SFML\Graphics.hpp"

#include <list>
#include <utility>
#include <functional>


class CenterTrackBase;


typedef std::pair<sf::Vector2f, sf::Vector2f> BorderTrackSegment;

class BorderTrackBase
{
private:
	std::list<BorderTrackSegment> mListOfBorderTrackSegments;

public:
	BorderTrackBase() = default;
	~BorderTrackBase() = default;
	BorderTrackBase(BorderTrackBase const &) = default;
	BorderTrackBase& operator=(BorderTrackBase const &) = default;

	BorderTrackBase(std::list<BorderTrackSegment> const & listOfBorderTrackSegments);


	CenterTrackBase getCenterTrackBase() const;

	BorderTrackBase& doForAllSectors(std::function<void(BorderTrackSegment &, BorderTrackSegment &)> task);

	template <typename T> std::list<T> calculateForAllSectorsAndPutIntoList(std::function<T(BorderTrackSegment const &, BorderTrackSegment const &)> task) const
	{
		std::list<T> list;
		if (!mListOfBorderTrackSegments.empty())
		{
			std::list<BorderTrackSegment>::const_iterator itOne = mListOfBorderTrackSegments.begin();
			std::list<BorderTrackSegment>::const_iterator itTwo = itOne;
			++itTwo;

			while (itTwo != mListOfBorderTrackSegments.end())
			{
				list.push_back(task(*itOne, *itTwo));
				++itOne;
				++itTwo;
			}
			list.push_back(task(mListOfBorderTrackSegments.back(), mListOfBorderTrackSegments.front()));
		}
		return list;
	}



};



#endif //BORDERTRACKBASE_HPP

