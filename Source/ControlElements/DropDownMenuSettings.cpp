#include "stdafx.h"
#include "Source\ControlElements\DropDownMenuSettings.hpp"

const DropDownMenuSettings::ColorSettings DropDownMenuSettings::ColorSettings::standardColorSettings = DropDownMenuSettings::ColorSettings
(
	//Normal of ActualChoice
	sf::Color(100, 100, 100),	//Background
	sf::Color(0, 0, 0),			//Outline
	sf::Color(20, 20, 20),		//Text

	//Inactive of ActualChoice
	sf::Color(50, 50, 50),
	sf::Color(0, 0, 0),
	sf::Color(20, 20, 20),

	//Normal of DropDown
	sf::Color(100, 100, 100),
	sf::Color(0, 0, 0),
	sf::Color(20, 20, 20),

	//Touched of DropDown
	sf::Color(130, 130, 130),
	sf::Color(30, 30, 30),
	sf::Color(50, 50, 50),

	//Triangle Colors
	sf::Color(10, 10, 10),		//Closed Background
	sf::Color(170, 170, 170),	//Closed Outline
	sf::Color(30, 30, 30),		//Open Background
	sf::Color(150, 150, 150),	//Open Outline
	sf::Color(60, 60, 60),		//Inactive Background
	sf::Color(20, 20, 20),		//Inactive Outline

	//Bar Colors
	sf::Color(200, 200, 200),		//Background Fill
	sf::Color(100, 100, 100),	//Background Outline
	sf::Color(150, 150, 150),		//Bar Fill
	sf::Color(80, 80, 80)	//Bar Outline

);

const DropDownMenuSettings DropDownMenuSettings::standardDropDownMenuSettings = DropDownMenuSettings(DropDownMenuSettings::ColorSettings::standardColorSettings);

