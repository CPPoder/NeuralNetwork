#ifndef RANDOMBRAIN_HPP
#define RANDOMBRAIN_HPP

#include "Source\Car\Brain.hpp"


class RandomBrain final : public Brain
{


public:
	RandomBrain() = default;
	virtual ~RandomBrain() override = default;
	RandomBrain(RandomBrain const &) = default;
	RandomBrain& operator=(RandomBrain const &) = default;

	virtual BrainType getBrainType() const override;

	virtual BrainOutput calculateBrainOutput(RaceSimulation const * raceSimPointer, Car const * carPointer) override;


};






#endif //RANDOMBRAIN_HPP

