#ifndef NETBRAIN_HPP
#define NETBRAIN_HPP

#include "Source\NeuralNet\SequentialNet.hpp"
#include "Source\Car\Brain.hpp"

#include <vector>


class NetBrain final : public Brain
{
private:
	SequentialNet mSequentialNet;

public:
	NetBrain();
	virtual ~NetBrain() override = default;
	NetBrain(NetBrain const &) = default;
	NetBrain& operator=(NetBrain const &) = default;

	virtual BrainType getBrainType() const override;

	virtual BrainOutput calculateBrainOutput(World const * worldPointer, Car const * carPointer) override;

public:
	static const std::vector<float> sVecOfAngles;
	static const std::vector<float> sFullVecOfAngles;


};




#endif //NETBRAIN_HPP

