#ifndef TEXTFIELDSETTINGS_HPP
#define TEXTFIELDSETTINGS_HPP


#include "SFML\Graphics.hpp"


struct TextFieldSettings
{
	struct ColorSettings
	{
		sf::Color normalBackgroundColor;
		sf::Color normalOutlineColor;
		sf::Color normalTextColor;

		sf::Color blinkingBackgroundColor;
		sf::Color blinkingOutlineColor;
		sf::Color blinkingTextColor;

		sf::Color inactiveBackgroundColor;
		sf::Color inactiveOutlineColor;
		sf::Color inactiveTextColor;

		ColorSettings(
			sf::Color const & _normalBackgroundColor,
			sf::Color const & _normalOutlineColor,
			sf::Color const & _normalTextColor,
			sf::Color const & _blinkingBackgroundColor,
			sf::Color const & _blinkingOutlineColor,
			sf::Color const & _blinkingTextColor,
			sf::Color const & _inactiveBackgroundColor,
			sf::Color const & _inactiveOutlineColor,
			sf::Color const & _inactiveTextColor
		)
			: normalBackgroundColor(_normalBackgroundColor),
			normalOutlineColor(_normalOutlineColor),
			normalTextColor(_normalTextColor),
			blinkingBackgroundColor(_blinkingBackgroundColor),
			blinkingOutlineColor(_blinkingOutlineColor),
			blinkingTextColor(_blinkingTextColor),
			inactiveBackgroundColor(_inactiveBackgroundColor),
			inactiveOutlineColor(_inactiveOutlineColor),
			inactiveTextColor(_inactiveTextColor)
		{}

		static const ColorSettings standardColorSettings;

	}; //Class ColorSettings

	ColorSettings colorSettings;

	TextFieldSettings(ColorSettings const & _colorSettings)
		: colorSettings(_colorSettings)
	{}

	static const TextFieldSettings standardTextFieldSettings;



}; //Class TextFieldSettings



#endif //TEXTFIELDSETTINGS_HPP

