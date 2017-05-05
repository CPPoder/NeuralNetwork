#include "stdafx.h"
#include "Source\ControlElements\Button.hpp"

//Constructor
Button::Button(sf::Vector2f const & position, sf::Vector2f const & size, std::string const & text, mySFML::Class::FontName fontName, float outlineThickness, unsigned int characterSize, bool active, ButtonSettings const * buttonSettings)
	: pButtonSettings(buttonSettings),
	  mPosition(position),
	  mSize(size),
	  mText(text, *mFonts.getFont(fontName), characterSize)
{
	this->setActive(active);

	this->setInternalObjectsPositions();
	this->setInternalObjectsColors();

	mRectangleShape.setSize(mSize);
	mRectangleShape.setOutlineThickness(outlineThickness);



}

//Destructor
Button::~Button()
{

}


//Update
void Button::updateState(sf::RenderWindow* renderWindow, sf::View const * view)
{
	if (mButtonState != ButtonState::INACTIVE)
	{
		sf::Vector2f mousePos;
		if (view == nullptr)
		{
			mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*renderWindow));
		}
		else
		{
			sf::View initialView = renderWindow->getView();
			renderWindow->setView(*view);
			mousePos = renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*renderWindow));
			renderWindow->setView(initialView);
		}
		if (mRectangleShape.getGlobalBounds().contains(mousePos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				this->changeStateTo(ButtonState::PRESSED);
			}
			else
			{
				this->changeStateTo(ButtonState::TOUCHED);
			}
		}
		else
		{
			this->changeStateTo(ButtonState::NORMAL);
		}
	}
}

//Render
void Button::render(sf::RenderWindow* renderWindow)
{
	renderWindow->draw(mRectangleShape);
	renderWindow->draw(mText);
}


//Setter
void Button::setPosition(sf::Vector2f const & position)
{
	mPosition = position;
	this->setInternalObjectsPositions();
}
void Button::move(sf::Vector2f const & move)
{
	mPosition += move;
	this->setInternalObjectsPositions();
}
void Button::setSize(sf::Vector2f const & size)
{
	mSize = size;
	this->setInternalObjectsPositions();
}
void Button::setButtonSettings(ButtonSettings const * buttonSettings)
{
	pButtonSettings = buttonSettings;
	this->setInternalObjectsColors();
}
void Button::setActive(bool active)
{
	if (active)
	{
		mButtonState = ButtonState::NORMAL;
	}
	else
	{
		mButtonState = ButtonState::INACTIVE;
	}
	this->setInternalObjectsColors();
}
void Button::setTextString(std::string const & string)
{
	mText.setString(string);
}


//Getter
sf::Vector2f Button::getPosition() const
{
	return mPosition;
}
sf::Vector2f Button::getSize() const
{
	return mSize;
}
ButtonSettings const * Button::getButtonSettings() const
{
	return pButtonSettings;
}
bool Button::getIsActive() const
{
	return (mButtonState != ButtonState::INACTIVE);
}
ButtonState Button::getButtonState() const
{
	return mButtonState;
}
std::string Button::getTextString() const
{
	return mText.getString();
}
bool Button::getMouseReleasedEventOccured(sf::RenderWindow* renderWindow, sf::View const * view) const
{
	if (mButtonState == ButtonState::INACTIVE)
	{
		return false;
	}
	if (EventManager::checkForEvent(EventManager::EventType::MOUSE_RELEASED))
	{
		if (EventManager::getReleasedMouseInfo().button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mousePos;
			if (view == nullptr)
			{
				mousePos = static_cast<sf::Vector2f>(EventManager::getReleasedMouseInfo().position);
			}
			else
			{
				sf::View initialView = renderWindow->getView();
				renderWindow->setView(*view);
				mousePos = renderWindow->mapPixelToCoords(EventManager::getReleasedMouseInfo().position);
				renderWindow->setView(initialView);
			}
			if (mRectangleShape.getGlobalBounds().contains(mousePos))
			{
				return true;
			}
		}
	}
	return false;
}


//Internal Manipulators
void Button::setInternalObjectsPositions()
{
	mRectangleShape.setPosition(mPosition);
	mText.setPosition(this->calculateTextPosition());
}
void Button::setInternalObjectsColors()
{
	switch (mButtonState)
	{
	case ButtonState::NORMAL:
		mRectangleShape.setFillColor(pButtonSettings->colorSettings.normalBackgroundColor);
		mRectangleShape.setOutlineColor(pButtonSettings->colorSettings.normalOutlineColor);
		mText.setFillColor(pButtonSettings->colorSettings.normalTextColor);
		break;
	case ButtonState::TOUCHED:
		mRectangleShape.setFillColor(pButtonSettings->colorSettings.touchedBackgroundColor);
		mRectangleShape.setOutlineColor(pButtonSettings->colorSettings.touchedOutlineColor);
		mText.setFillColor(pButtonSettings->colorSettings.touchedTextColor);
		break;
	case ButtonState::PRESSED:
		mRectangleShape.setFillColor(pButtonSettings->colorSettings.pressedBackgroundColor);
		mRectangleShape.setOutlineColor(pButtonSettings->colorSettings.pressedOutlineColor);
		mText.setFillColor(pButtonSettings->colorSettings.pressedTextColor);
		break;
	case ButtonState::INACTIVE:
		mRectangleShape.setFillColor(pButtonSettings->colorSettings.inactiveBackgroundColor);
		mRectangleShape.setOutlineColor(pButtonSettings->colorSettings.inactiveOutlineColor);
		mText.setFillColor(pButtonSettings->colorSettings.inactiveTextColor);
		break;
	}
}

void Button::changeStateTo(ButtonState buttonState)
{
	if (mButtonState != buttonState)
	{
		mButtonState = buttonState;
		this->setInternalObjectsColors();
	}
}

sf::Vector2f Button::calculateTextPosition() const
{
	sf::FloatRect globalBoundsText = mText.getGlobalBounds();
	sf::Vector2f sizeOfText = sf::Vector2f(globalBoundsText.width, globalBoundsText.height);
	sf::Vector2f relativePos = (mSize - sizeOfText) / 2.f;
	sf::Vector2f correction = sf::Vector2f(0.f, -sizeOfText.y * 0.4f);
	return (mPosition + relativePos + correction);
}

