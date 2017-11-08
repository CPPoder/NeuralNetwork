#ifndef BRAINOUTPUT_HPP
#define BRAINOUTPUT_HPP


struct BrainOutput
{
public:
	float gasBrakeFactorDerivative;
	float steeringWheelFactorDerivative;

public:
	BrainOutput();
	BrainOutput(float _gasBrakeFactorDerivative, float _steeringWheelFactorDerivative);
	~BrainOutput() = default;
	BrainOutput(BrainOutput const &) = default;
	BrainOutput& operator=(BrainOutput const &) = default;



};




#endif //BRAINOUTPUT_HPP

