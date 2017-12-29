#ifndef NETBRAIN_HPP
#define NETBRAIN_HPP

#include "Source\NeuralNet\SequentialNet.hpp"
#include "Source\Car\Brain.hpp"

#include <vector>
#include <array>

#include "Source\Collision\Line.hpp"


class NetBrain final : public Brain
{
private:
	SequentialNet mSequentialNet;
	std::list<Line> mListOfSeeingLines;

public:
	NetBrain();
	virtual ~NetBrain() override = default;
	NetBrain(NetBrain const & netBrain);
	NetBrain& operator=(NetBrain const & netBrain);

	virtual BrainType getBrainType() const override;

	virtual BrainOutput calculateBrainOutput(World const * worldPointer, Car const * carPointer) override;

	void mutate();
	void setEntriesRandom();

public:
	void renderSeeingLines(sf::RenderWindow* renderWindow, sf::View view) const;
	void saveToFile(std::string const & path) const;
	void loadFromFile(std::string const & path);

public:
	static constexpr unsigned int sNumOfAngles = 3u; //7 different angles
	static constexpr unsigned int sFullNumOfAngles = 2u * sNumOfAngles + 1u; //Every angle in both directions + front
	static constexpr unsigned int sNetInputSize = sFullNumOfAngles + 4u; //borderCollisions + forwardProjection + (velocity, actualGasBrake, actualSteering)
	static constexpr unsigned int sNetOutputSize = 2u; //gasBrakeDerivative & steeringWheelDerivative
	static const std::array<float, sNumOfAngles> sArrayOfAngles;
	static const std::array<float, sFullNumOfAngles> sFullArrayOfAngles;


};




#endif //NETBRAIN_HPP

