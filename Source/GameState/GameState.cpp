#include "stdafx.h"
#include "GameState.hpp"

namespace GameState
{

	Change::Change(Type _type, State _toState)
		: type(_type),
		  toState(_toState)
	{
	}

}

