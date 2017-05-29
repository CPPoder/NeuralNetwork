#ifndef CENTERTRACKBASE_HPP
#define CENTERTRACKBASE_HPP


#include "SFML\Graphics.hpp"

#include <list>
#include <utility>
#include <functional>


class BorderTrackBase;
class Track;


typedef std::pair<sf::Vector2f, sf::Vector2f> CenterTrackSegment;

class CenterTrackBase
{
private:
	std::list<CenterTrackSegment> mListOfCenterTrackSegments;

public:
	CenterTrackBase() = default;
	~CenterTrackBase() = default;
	CenterTrackBase(CenterTrackBase const &) = default;
	CenterTrackBase& operator=(CenterTrackBase const &) = default;

	CenterTrackBase(std::list<CenterTrackSegment> const & listOfCenterTrackSegments);

	typedef std::list<CenterTrackSegment>::iterator iterator;
	typedef std::list<CenterTrackSegment>::const_iterator const_iterator;
	iterator begin() { return mListOfCenterTrackSegments.begin(); }
	iterator end() { return mListOfCenterTrackSegments.end(); }


	BorderTrackBase getBorderTrackBase() const;


	std::list<CenterTrackSegment> const & getListOfCenterTrackSegments() const;
	void setListOfCenterTrackSegments(std::list<CenterTrackSegment> const & listOfCenterTrackSegments);
	

	CenterTrackBase& doForAllSectors(std::function<void(CenterTrackSegment &, CenterTrackSegment &)> task);

	template <typename T> std::list<T> calculateForAllSectorsAndPutIntoList(std::function<T(CenterTrackSegment const &, CenterTrackSegment const &)> task) const
	{
		std::list<T> list;
		if (!mListOfCenterTrackSegments.empty())
		{
			std::list<CenterTrackSegment>::const_iterator itOne = mListOfCenterTrackSegments.begin();
			std::list<CenterTrackSegment>::const_iterator itTwo = itOne;
			++itTwo;

			while (itTwo != mListOfCenterTrackSegments.end())
			{
				list.push_back(task(*itOne, *itTwo));
				++itOne;
				++itTwo;
			}
			list.push_back(task(mListOfCenterTrackSegments.back(), mListOfCenterTrackSegments.front()));
		}
		return list;
	}

	void setRelativeVectorsOrthogonal();


};


#endif //CENTERTRACKBASE_HPP

