#include "stdafx.h"
#include "Source\ControlElements\ButtonSettings.hpp"

const ButtonSettings::ColorSettings ButtonSettings::ColorSettings::standardColorSettings = ButtonSettings::ColorSettings
(
	//Normal
	sf::Color(100, 100, 100),	//Background
	sf::Color(0, 0, 0),	//Outline
	sf::Color(20, 20, 20),	//Text

	//Touched
	sf::Color(130, 130, 130),
	sf::Color(30, 30, 30),
	sf::Color(50, 50, 50),

	//Pressed
	sf::Color(160, 160, 160),
	sf::Color(30, 30, 30),
	sf::Color(80, 80, 80),

	//Inactive
	sf::Color(50, 50, 50),
	sf::Color(0, 0, 0),
	sf::Color(20, 20, 20)
);

const ButtonSettings ButtonSettings::standardButtonSettings = ButtonSettings(ButtonSettings::ColorSettings::standardColorSettings);

