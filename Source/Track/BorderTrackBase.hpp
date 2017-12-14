#ifndef BORDERTRACKBASE_HPP
#define BORDERTRACKBASE_HPP


#include "SFML\Graphics.hpp"

#include <list>
#include <utility>
#include <functional>
#include <iostream>


class CenterTrackBase;
class Track;


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

	typedef std::list<BorderTrackSegment>::iterator iterator;
	typedef std::list<BorderTrackSegment>::const_iterator const_iterator;
	iterator begin() { return mListOfBorderTrackSegments.begin(); }
	iterator end() { return mListOfBorderTrackSegments.end(); }
	const_iterator cbegin() const { return mListOfBorderTrackSegments.cbegin(); }
	const_iterator cend() const { return mListOfBorderTrackSegments.cend(); }
	const_iterator begin() const { return this->cbegin(); }
	const_iterator end() const { return this->cend(); }

	BorderTrackBase::const_iterator getIteratorToNearestSegment(sf::Vector2f const & pos) const;

	void addSegmentBefore(std::list<BorderTrackSegment>::iterator pos, BorderTrackSegment const & segment) { mListOfBorderTrackSegments.insert(pos, segment); }

	CenterTrackBase getCenterTrackBase() const;

	std::list<BorderTrackSegment> const & getListOfBorderTrackSegments() const;
	void setListOfBorderTrackSegments(std::list<BorderTrackSegment> const & listOfBorderTrackSegments);


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

	friend std::ostream& operator<<(std::ostream & stream, BorderTrackBase const & borderTrackBase);

};


std::ostream& operator<<(std::ostream & stream, BorderTrackBase const & borderTrackBase);



#endif //BORDERTRACKBASE_HPP

