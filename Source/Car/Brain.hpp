#ifndef BRAIN_HPP
#define BRAIN_HPP

#include "SFML\Graphics.hpp"

#include "Source\Car\BrainOutput.hpp"
#include "Source\Car\BrainType.hpp"

class RaceSimulation;
class Car;


class Brain
{
private:
	
public:
	Brain() = default;
	virtual ~Brain() = default;
	Brain(Brain const &) = default;
	Brain& operator=(Brain const &) = default;

	virtual BrainType getBrainType() const = 0;

	virtual BrainOutput calculateBrainOutput(RaceSimulation const * raceSimPointer, Car const * carPointer) = 0;

	static Brain* constructBrain(BrainType brainType);

};





#endif //BRAIN_HPP

