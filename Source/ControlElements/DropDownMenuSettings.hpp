#ifndef DROPDOWNMENUSETTINGS_HPP
#define DROPDOWNMENUSETTINGS_HPP


#include "SFML\Graphics.hpp"


struct DropDownMenuSettings
{
	struct ColorSettings
	{
		//Normal Color of ActualChoiceRectangle
		sf::Color normalActualChoiceBackgroundColor;
		sf::Color normalActualChoiceOutlineColor;
		sf::Color normalActualChoiceTextColor;

		//Inactive Color of ActualChoiceRectangle
		sf::Color inactiveActualChoiceBackgroundColor;
		sf::Color inactiveActualChoiceOutlineColor;
		sf::Color inactiveActualChoiceTextColor;

		//Normal Color of DropDownRectangles
		sf::Color normalDropDownBackgroundColor;
		sf::Color normalDropDownOutlineColor;
		sf::Color normalDropDownTextColor;

		//Touched Color of DropDownRectangles
		sf::Color touchedDropDownBackgroundColor;
		sf::Color touchedDropDownOutlineColor;
		sf::Color touchedDropDownTextColor;

		//Color of Triangle
		sf::Color closedTriangleBackgroundColor;
		sf::Color closedTriangleOutlineColor;
		sf::Color openTriangleBackgroundColor;
		sf::Color openTriangleOutlineColor;
		sf::Color inactiveTriangleBackgroundColor;
		sf::Color inactiveTriangleOutlineColor;

		//Color of Bar
		sf::Color barBackgroundFillColor;
		sf::Color barBackgroundOutlineColor;
		sf::Color barFillColor;
		sf::Color barOutlineColor;


		ColorSettings(
			sf::Color const & _normalActualChoiceBackgroundColor,
			sf::Color const & _normalActualChoiceOutlineColor,
			sf::Color const & _normalActualChoiceTextColor,
			sf::Color const & _inactiveActualChoiceBackgroundColor,
			sf::Color const & _inactiveActualChoiceOutlineColor,
			sf::Color const & _inactiveActualChoiceTextColor,
			sf::Color const & _normalDropDownBackgroundColor,
			sf::Color const & _normalDropDownOutlineColor,
			sf::Color const & _normalDropDownTextColor,
			sf::Color const & _touchedDropDownBackgroundColor,
			sf::Color const & _touchedDropDownOutlineColor,
			sf::Color const & _touchedDropDownTextColor,
			sf::Color const & _closedTriangleBackgroundColor,
		    sf::Color const & _closedTriangleOutlineColor,
		    sf::Color const & _openTriangleBackgroundColor,
		    sf::Color const & _openTriangleOutlineColor,
			sf::Color const & _inactiveTriangleBackgroundColor,
			sf::Color const & _inactiveTriangleOutlineColor,
			sf::Color const & _barBackgroundFillColor,
			sf::Color const & _barBackgroundOutlineColor,
			sf::Color const & _barFillColor,
			sf::Color const & _barOutlineColor
		)
			: normalActualChoiceBackgroundColor(_normalActualChoiceBackgroundColor),
			normalActualChoiceOutlineColor(_normalActualChoiceOutlineColor),
			normalActualChoiceTextColor(_normalActualChoiceTextColor),
			inactiveActualChoiceBackgroundColor(_inactiveActualChoiceBackgroundColor),
			inactiveActualChoiceOutlineColor(_inactiveActualChoiceOutlineColor),
			inactiveActualChoiceTextColor(_inactiveActualChoiceTextColor),
			normalDropDownBackgroundColor(_normalDropDownBackgroundColor),
			normalDropDownOutlineColor(_normalDropDownOutlineColor),
			normalDropDownTextColor(_normalDropDownTextColor),
			touchedDropDownBackgroundColor(_touchedDropDownBackgroundColor),
			touchedDropDownOutlineColor(_touchedDropDownOutlineColor),
			touchedDropDownTextColor(_touchedDropDownTextColor),
			closedTriangleBackgroundColor(_closedTriangleBackgroundColor),
			closedTriangleOutlineColor(_closedTriangleOutlineColor),
			openTriangleBackgroundColor(_openTriangleBackgroundColor),
			openTriangleOutlineColor(_openTriangleOutlineColor),
			inactiveTriangleBackgroundColor(_inactiveTriangleBackgroundColor),
			inactiveTriangleOutlineColor(_inactiveTriangleOutlineColor),
			barBackgroundFillColor(_barBackgroundFillColor),
			barBackgroundOutlineColor(_barBackgroundOutlineColor),
			barFillColor(_barFillColor),
			barOutlineColor(_barOutlineColor)
		{}

		static const ColorSettings standardColorSettings;

	}; //Class ColorSettings

	ColorSettings colorSettings;

	DropDownMenuSettings(ColorSettings const & _colorSettings)
		: colorSettings(_colorSettings)
	{}

	static const DropDownMenuSettings standardDropDownMenuSettings;



}; //Class ButtonSettings







#endif //DROPDOWNMENUSETTINGS_HPP

