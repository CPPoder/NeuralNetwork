#include "stdafx.h"
#include "Source\Car\Brain.hpp"

#include "Source\Car\PlayerBrain.hpp"

Brain* Brain::constructBrain(BrainType brainType)
{
	switch (brainType)
	{
	case BrainType::PLAYER:
		return new PlayerBrain;
	}
}
