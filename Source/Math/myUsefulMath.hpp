#ifndef MYUSEFULMATH_HPP
#define MYUSEFULMATH_HPP

#include <vector>
#include <iostream>
#include <functional>

namespace myMath {

	namespace Const
	{

		extern const float SQRT2f;
		extern const double SQRT2d;
		extern const float PIf;
		extern const double PId;

	} //Namespace Const
	
	namespace Simple
	{

		template <typename T> T max(T num1, T num2)
		{
			if (num1 > num2)
			{
				return num1;
			}
			else
			{
				return num2;
			}
		}

		template <typename T> T min(T num1, T num2)
		{
			if (num1 > num2)
			{
				return num2;
			}
			else
			{
				return num1;
			}
		}

		template <typename T> T trim(T lowerBound, T value, T upperBound)
		{
			if (value < lowerBound)
			{
				return lowerBound;
			}
			if (value > upperBound)
			{
				return upperBound;
			}
			return value;
		}

		template <typename T> T abs(T value)
		{
			if (value < T(0))
			{
				return (-value);
			}
			else
			{
				return value;
			}
		}

		template <typename T> bool checkIfIntervalsOverlap(T interval1Min, T interval1Max, T interval2Min, T interval2Max, T & newIntervalMin, T & newIntervalMax)
		{
			newIntervalMin = myMath::Simple::max(interval1Min, interval2Min);
			newIntervalMax = myMath::Simple::min(interval1Max, interval2Max);
			return (newIntervalMin <= newIntervalMax);
		}

		float meanValue(float num1, float num2);
		int sign(int value);
		float sign(float value);
		int intPow(int val, unsigned int exp);

	} //Namespace Simple

	namespace Rand
	{

		int randIntervali(int small, int big);
		float randIntervalf(int small, int big);
		bool randBool();

	} //Namespace Rand

	namespace Trafo
	{

		unsigned int trafoBoolVectorToUInt(std::vector<bool> input);
		std::vector<bool> trafoUIntToBoolVector(unsigned int input);
		std::vector<bool> trafoUIntToBoolVectorOfGivenSize(unsigned int input, unsigned int size);

		//IntervalBasedAffineTrafo
		template <typename T, typename S> S intervalBasedAffineTrafo(T domainBegin, T domainEnd, S imageBegin, S imageEnd, T input)
		{
			//Cast to S
			S dB = static_cast<S>(domainBegin);
			S dE = static_cast<S>(domainEnd);
			S iB = imageBegin;
			S iE = imageEnd;
			S in = static_cast<S>(input);

			//Check for empty Intervals
			if ((iB == iE) || (dB == dE))
			{
				std::cerr << "Error: S myMath::intervalBasedAffineTrafo(T, T, S, S) : Domain or Image are empty!" << std::endl;
				throw std::bad_exception();
				return S(0);
			}

			//Calculate Transformation Parameters
			S m = (iE - iB) / (dE - dB);
			S t = iB - (m * dB);

			//Perform Transformation
			return ((m * in) + t);
		}

		//CreateIntervalBasedAffineTrafo
		template <typename T, typename S> std::function<S(T)> createIntervalBasedAffineTrafo(T domainBegin, T domainEnd, S imageBegin, S imageEnd)
		{
			//Cast to S
			S dB = static_cast<S>(domainBegin);
			S dE = static_cast<S>(domainEnd);
			S iB = imageBegin;
			S iE = imageEnd;

			//Check for empty Intervals
			if ((iB == iE) || (dB == dE))
			{
				std::cerr << "Error: S myMath::intervalBasedAffineTrafo(T, T, S, S) : Domain or Image are empty!" << std::endl;
				throw std::bad_exception();
				return std::function<S(T)>();
			}

			//Calculate Transformation Parameters
			S m = (iE - iB) / (dE - dB);
			S t = iB - (m * dB);

			//Return Function
			std::function<S(T)> trafo = [=](T input) {
				S in = static_cast<S>(input);
				return ((m * in) + t);
			};
			return trafo;
		}

	} //Namespace Trafo	
	
} //Namespace myMath


#endif //MYUSEFULMATH_HPP
