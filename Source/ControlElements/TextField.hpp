#ifndef TEXTFIELD_HPP
#define TEXTFIELD_HPP


#include "mySFMLVectorFunctions.hpp"

#include "Source\ControlElements\TextFieldSettings.hpp"
#include "Source\Framework\EventManager.hpp"

#include <string>


enum class TextFieldState
{
	NORMAL,
	BLINKING,
	INACTIVE
};

enum class InputBehaviour
{
	FREE,
	BOUNDED_FROM_BELOW
};


class TextField
{
private:
	mySFML::Class::Fonts mFonts;

	TextFieldSettings const * pTextFieldSettings;
	InputBehaviour mInputBehaviour;
	unsigned int mInputBehaviourArgument;
	sf::Vector2f mPosition;
	sf::Vector2f mSize;
	TextFieldState mTextFieldState;

	sf::RectangleShape mRectangleShape;
	sf::Text mText;
	mySFML::Class::RectShape mBlinkingLineRect;

	sf::Clock mBlinkingClock;
	sf::Time const mBlinkingTime = sf::seconds(0.5f);
	bool mBlinkingLineVisible = true;


public:
	TextField(sf::Vector2f const & position, sf::Vector2f const & size, std::string const & text, mySFML::Class::FontName fontName, float outlineThickness = 2.f, unsigned int characterSize = 12u, bool active = true, InputBehaviour inputBehaviour = InputBehaviour::FREE, unsigned int behaviourArgument = 0, TextFieldSettings const * textFieldSettings = &TextFieldSettings::standardTextFieldSettings);
	~TextField();

	void updateState(sf::RenderWindow* renderWindow, sf::View const * view = nullptr); //Pointer, beacause I need to have some default argument
	void render(sf::RenderWindow* renderWindow);

	void setPosition(sf::Vector2f const & position);
	void move(sf::Vector2f const & move);
	void setSize(sf::Vector2f const & size);
	void setTextFieldSettings(TextFieldSettings const * textFieldSettings);
	void setActive(bool active);
	void setTextString(std::string const & string);

	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	TextFieldSettings const * getTextFieldSettings() const;
	bool getIsActive() const;
	TextFieldState getTextFieldState() const;
	std::string getTextString() const;
	unsigned int getTextStringSize() const;

private:
	void setInternalObjectsPositions();
	void setInternalObjectsColors();

	void changeStateTo(TextFieldState textFieldState);

	sf::Vector2f calculateTextPosition() const;
	sf::Vector2f calculateBlinkingLinePosition() const;
	sf::Vector2f calculateBlinkingLineSize() const;
};




#endif //TEXTFIELD_HPP

