#include "stdafx.h"
#include "Source\ControlElements\TextFieldSettings.hpp"

const TextFieldSettings::ColorSettings TextFieldSettings::ColorSettings::standardColorSettings = TextFieldSettings::ColorSettings
(
	//Normal
	sf::Color(100, 100, 100),	//Background
	sf::Color(0, 0, 0),	//Outline
	sf::Color(20, 20, 20),	//Text

	//Blinking
	sf::Color(130, 130, 130),
	sf::Color(30, 30, 30),
	sf::Color(50, 50, 50),

	//Inactive
	sf::Color(50, 50, 50),
	sf::Color(0, 0, 0),
	sf::Color(20, 20, 20)
);

const TextFieldSettings TextFieldSettings::standardTextFieldSettings = TextFieldSettings(TextFieldSettings::ColorSettings::standardColorSettings);

