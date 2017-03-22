#include "stdafx.h"
#include "myUsefulMath.hpp"

#include <cstdlib>


namespace myMath {

	namespace Const
	{

		const float SQRT2f = 1.41421356f;
		const double SQRT2d = 1.4142135623730950488;
		const float PIf = 3.141592653f;
		const double PId = 3.14159265358979323846;

	} //Namespace Const

	namespace Simple
	{

		////////////
		//Mean Value
		float meanValue(float num1, float num2)
		{
			return ((num1 + num2) / 2.f);
		}

		//////
		//Sign
		int sign(int value)
		{
			if (value < 0)
			{
				return (-1);
			}
			else
			{
				return 1;
			}
		}

		//////
		//Sign
		float sign(float value)
		{
			if (value < 0)
			{
				return (-1.f);
			}
			else
			{
				return 1.f;
			}
		}

		////////
		//IntPow
		int intPow(int val, unsigned int exp)
		{
			if (val == 0 && exp == 0)
			{
				std::cerr << "Error: intPow(int, unsigned int) should calculate 0^0!" << std::endl;
				throw std::bad_exception();
				return 0;
			}
			if (val == 0)
			{
				return 0;
			}
			if (exp == 0)
			{
				return 1;
			}
			//All other cases arrive here
			int output = 1;
			for (unsigned int prodNum = 0; prodNum < exp; prodNum++)
			{
				output = output * val;
			}
			return output;
		}

	} //Namespace Simple

	namespace Rand
	{

		int randIntervali(int small, int big)
		{
			return (rand() % (big - small + 1) + small);
		}

		float randIntervalf(int small, int big)
		{
			return static_cast<float>(randIntervali(small, big));
		}

		bool randBool()
		{
			return static_cast<bool>(randIntervali(0, 1));
		}

	} //Namespace Rand

	namespace Trafo
	{

		///////////////////////
		//TrafoBoolVectorToUInt
		unsigned int trafoBoolVectorToUInt(std::vector<bool> input)
		{
			if (input.empty())
			{
				std::cerr << "Warning: myMath::trafoBoolVectorToUInt(std::vector<bool>) : Input Vector is empty!" << std::endl;
				return 0;
			}
			unsigned int output = 0;
			unsigned int bitPos = input.size() - 1;
			unsigned int bitWeight = 1;
			bool firstIteration = true;
			while (bitPos != 0)
			{
				if (firstIteration)
				{
					firstIteration = false;
				}
				else
				{
					bitPos = bitPos - 1;
					bitWeight = bitWeight * 2;
				}
				if (input.at(bitPos))
				{
					output = output + bitWeight;
				}
			}
			return output;
		}

		///////////////////////
		//TrafoUIntToBoolVector
		std::vector<bool> trafoUIntToBoolVector(unsigned int input)
		{
			//Handle Special Cases
			if (input == 0)
			{
				return std::vector<bool>(1, false);
			}
			if (input == 1)
			{
				return std::vector<bool>(1, true);
			}

			//Determine Size
			unsigned int size = 1;
			unsigned int testingValue = 2;
			while (testingValue <= input)
			{
				testingValue = testingValue * 2;
				size = size + 1;
			}
			std::vector<bool> output(size, false);
			output.at(0) = true;
			output.shrink_to_fit();

			//Determine the Bits
			unsigned int maxBitVal = myMath::Simple::intPow(2, (size - 1));
			unsigned int remainingValue = input - maxBitVal;
			unsigned int currentBitPos = 0;
			unsigned int currentBitVal = maxBitVal;
			bool calculationNotFinished = true;
			if (remainingValue == 0)
			{
				calculationNotFinished = false;
			}
			while (calculationNotFinished)
			{
				currentBitPos = currentBitPos + 1;
				currentBitVal = currentBitVal / 2;
				if (currentBitVal <= remainingValue)
				{
					output.at(currentBitPos) = true;
					remainingValue = remainingValue - currentBitVal;
				}
				if (remainingValue == 0)
				{
					calculationNotFinished = false;
				}
			}

			//Return Output
			return output;
		}

		//////////////////////////////////
		//TrafoUIntToBoolVectorOfGivenSize
		std::vector<bool> trafoUIntToBoolVectorOfGivenSize(unsigned int input, unsigned int size)
		{
			std::vector<bool> result = trafoUIntToBoolVector(input);
			unsigned int resultSize = result.size();
			unsigned int difference = size - resultSize;
			if (difference == 0)
			{
				return result;
			}
			if (difference < 0)
			{
				std::cerr << "Error: myMath::trafoUIntToBoolVectorOfGivenSize(unsigned int, unsigned int) : The input can not be represented by a binary number of size " << size << "!" << std::endl;
				return std::vector<bool>(size, true);
			}
			if (difference > 0)
			{
				std::vector<bool> output(difference, false);
				output.insert(output.end(), result.begin(), result.end());
				output.shrink_to_fit();
				return output;
			}
		}

	} //Namespace Trafo

} //Namespace myMath
