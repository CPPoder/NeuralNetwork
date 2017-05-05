#ifndef BUTTONSETTINGS_HPP
#define BUTTONSETTINGS_HPP


#include "SFML\Graphics.hpp"


struct ButtonSettings
{
	struct ColorSettings
	{
		sf::Color normalBackgroundColor;
		sf::Color normalOutlineColor;
		sf::Color normalTextColor;

		sf::Color touchedBackgroundColor;
		sf::Color touchedOutlineColor;
		sf::Color touchedTextColor;

		sf::Color pressedBackgroundColor;
		sf::Color pressedOutlineColor;
		sf::Color pressedTextColor;

		sf::Color inactiveBackgroundColor;
		sf::Color inactiveOutlineColor;
		sf::Color inactiveTextColor;

		ColorSettings(
			sf::Color const & _normalBackgroundColor,
			sf::Color const & _normalOutlineColor,
			sf::Color const & _normalTextColor,
			sf::Color const & _touchedBackgroundColor,
			sf::Color const & _touchedOutlineColor,
			sf::Color const & _touchedTextColor,
			sf::Color const & _pressedBackgroundColor,
			sf::Color const & _pressedOutlineColor,
			sf::Color const & _pressedTextColor,
			sf::Color const & _inactiveBackgroundColor,
			sf::Color const & _inactiveOutlineColor,
			sf::Color const & _inactiveTextColor
		)
			: normalBackgroundColor(_normalBackgroundColor),
			normalOutlineColor(_normalOutlineColor),
			normalTextColor(_normalTextColor),
			touchedBackgroundColor(_touchedBackgroundColor),
			touchedOutlineColor(_touchedOutlineColor),
			touchedTextColor(_touchedTextColor),
			pressedBackgroundColor(_pressedBackgroundColor),
			pressedOutlineColor(_pressedOutlineColor),
			pressedTextColor(_pressedTextColor),
			inactiveBackgroundColor(_inactiveBackgroundColor),
			inactiveOutlineColor(_inactiveOutlineColor),
			inactiveTextColor(_inactiveTextColor)
		{}

		static const ColorSettings standardColorSettings;

	}; //Class ColorSettings

	ColorSettings colorSettings;

	ButtonSettings(ColorSettings const & _colorSettings)
		: colorSettings(_colorSettings)
	{}

	static const ButtonSettings standardButtonSettings;
	


}; //Class ButtonSettings




#endif //BUTTONSETTINGS_HPP

