#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>


class Random
{
public:
	Random() = delete;
	~Random() = delete;

public:
	static int getRandomIntBetween(int first, int last);

	static float getRandomFloatBetween(float first, float last);
	static double getRandomDoubleBetween(double first, double last);
	

};


#endif //RANDOM_HPP

