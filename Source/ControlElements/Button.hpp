#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "mySFMLVectorFunctions.hpp"

#include "Source\ControlElements\ButtonSettings.hpp"
#include "Source\Framework\EventManager.hpp"

#include <string>


enum class ButtonState
{
	NORMAL,
	TOUCHED,
	PRESSED,
	INACTIVE
};


class Button
{
private:
	mySFML::Class::Fonts mFonts;

	ButtonSettings const * pButtonSettings;
	sf::Vector2f mPosition;
	sf::Vector2f mSize;
	ButtonState mButtonState;

	sf::RectangleShape mRectangleShape;
	sf::Text mText;


public:
	Button(sf::Vector2f const & position, sf::Vector2f const & size, std::string const & text, mySFML::Class::FontName fontName, float outlineThickness = 2.f, unsigned int characterSize = 12u, bool active = true, ButtonSettings const * buttonSettings = &ButtonSettings::standardButtonSettings);
	~Button();

	void updateState(sf::RenderWindow* renderWindow, sf::View const * view = nullptr); //Pointer, beacause I need to have some default argument);
	void render(sf::RenderWindow* renderWindow);

	void setPosition(sf::Vector2f const & position);
	void move(sf::Vector2f const & move);
	void setSize(sf::Vector2f const & size);
	void setButtonSettings(ButtonSettings const * buttonSettings);
	void setActive(bool active);
	void setTextString(std::string const & string);

	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	ButtonSettings const * getButtonSettings() const;
	bool getIsActive() const;
	ButtonState getButtonState() const;
	std::string getTextString() const;
	bool getMouseReleasedEventOccured(sf::RenderWindow* renderWindow = nullptr, sf::View const * view = nullptr) const;


private:
	void setInternalObjectsPositions();
	void setInternalObjectsColors();
	
	void changeStateTo(ButtonState buttonState);

	sf::Vector2f calculateTextPosition() const;

};





#endif //BUTTON_HPP

