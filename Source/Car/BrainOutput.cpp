#include "stdafx.h"
#include "Source\Car\BrainOutput.hpp"


BrainOutput::BrainOutput()
	: BrainOutput(0.f, 0.f)
{
}

BrainOutput::BrainOutput(float _gasBrakeFactorDerivative, float _steeringWheelFactorDerivative)
	: gasBrakeFactorDerivative(_gasBrakeFactorDerivative),
	  steeringWheelFactorDerivative(_steeringWheelFactorDerivative)
{
}

