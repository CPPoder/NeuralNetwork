#include "stdafx.h"
#include "Source\Car\BrainOutput.hpp"


BrainOutput::BrainOutput()
	: BrainOutput(0.f, 0.f)
{
}

BrainOutput::BrainOutput(float _gasBrakeForceDerivative, float _steeringWheelAngleDerivative)
	: gasBrakeForceDerivative(_gasBrakeForceDerivative),
	  steeringWheelAngleDerivative(_steeringWheelAngleDerivative)
{
}

