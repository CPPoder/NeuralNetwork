#ifndef PLAYERBRAIN_HPP
#define PLAYERBRAIN_HPP

#include "Source\Car\Brain.hpp"


class PlayerBrain final : public Brain
{


public:
	PlayerBrain() = default;
	virtual ~PlayerBrain() override = default;
	PlayerBrain(PlayerBrain const &) = default;
	PlayerBrain& operator=(PlayerBrain const &) = default;

	virtual BrainType getBrainType() const override;

	virtual BrainOutput calculateBrainOutput(World const * worldPointer, Car const * carPointer) override;


};






#endif //PLAYERBRAIN_HPP

