#ifndef BRAINOUTPUT_HPP
#define BRAINOUTPUT_HPP


struct BrainOutput
{
public:
	float gasBrakeForceDerivative;
	float steeringWheelAngleDerivative;

public:
	BrainOutput();
	BrainOutput(float _gasBrakeForceDerivative, float _steeringWheelAngleDerivative);
	~BrainOutput() = default;
	BrainOutput(BrainOutput const &) = default;
	BrainOutput& operator=(BrainOutput const &) = default;



};




#endif //BRAINOUTPUT_HPP

