#include "stdafx.h"
#include "Source\Framework\WindowChange.hpp"

WindowChange::Info::Info(sf::Vector2u const & _windowSize, bool _adjustView, sf::Vector2i _windowPosition)
	: windowSize(_windowSize), adjustView(_adjustView), windowPosition(_windowPosition)
{

}

WindowChange::WindowChange(Type _type, Info _info)
	: type(_type), info(_info)
{

}

