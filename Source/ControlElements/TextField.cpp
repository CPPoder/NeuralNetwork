#include "stdafx.h"
#include "Source\ControlElements\TextField.hpp"


//Constructor
TextField::TextField(sf::Vector2f const & position, sf::Vector2f const & size, std::string const & text, mySFML::Class::FontName fontName, float outlineThickness, unsigned int characterSize, bool active, InputBehaviour inputBehaviour, unsigned int behaviourArgument, TextFieldSettings const * textFieldSettings)
	: pTextFieldSettings(textFieldSettings),
	  mInputBehaviour(inputBehaviour),
	  mInputBehaviourArgument(behaviourArgument),
	  mPosition(position),
	  mSize(size),
	  mText(text, *mFonts.getFont(fontName), characterSize),
	  mBlinkingLineRect(this->calculateBlinkingLinePosition(), this->calculateBlinkingLineSize(), pTextFieldSettings->colorSettings.blinkingTextColor, false)
{
	this->setActive(active);

	this->setInternalObjectsPositions();
	this->setInternalObjectsColors();

	//mBlinkingLineRect.pointer->setPosition(this->calculateBlinkingLinePosition());
	//mBlinkingLineRect.pointer->setSize(this->calculateBlinkingLineSize());

	mRectangleShape.setSize(mSize);
	mRectangleShape.setOutlineThickness(outlineThickness);
}

//Destructor
TextField::~TextField()
{

}


//Update
void TextField::updateState(sf::RenderWindow* renderWindow, sf::View const * view)
{
	if (mTextFieldState != TextFieldState::INACTIVE)
	{
		//Manage Selection of TextField
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
					this->changeStateTo(TextFieldState::BLINKING);
				}
				else
				{
					this->changeStateTo(TextFieldState::NORMAL);
				}
			}
		}
		if (EventManager::checkForEvent(EventManager::EventType::KEY_RELEASED) && mTextFieldState == TextFieldState::BLINKING)
		{
			if (EventManager::getReleasedKeyInfo().key == sf::Keyboard::Key::Return)
			{
				this->changeStateTo(TextFieldState::NORMAL);
			}
		}

		//Manage Blinking
		if (mBlinkingClock.getElapsedTime() > mBlinkingTime)
		{
			mBlinkingClock.restart();
			mBlinkingLineVisible = !mBlinkingLineVisible;
		}

		//Change String
		if (mTextFieldState == TextFieldState::BLINKING)
		{
			if (EventManager::checkForEvent(EventManager::EventType::TEXT_ENTERED))
			{
				std::string enteredText = EventManager::getTextEnteredText(EventManager::TextMode::FILTER_FOR_PRINTABLE_ASCII);
				std::string existingText = mText.getString();
				existingText.append(enteredText);
				mText.setString(existingText);
				this->setInternalObjectsPositions();
			}
			if (EventManager::checkForEvent(EventManager::EventType::KEY_PRESSED))
			{
				bool inputBehaviourAllowsBackSpace = mInputBehaviour == InputBehaviour::FREE || ((mInputBehaviour == InputBehaviour::BOUNDED_FROM_BELOW) && (mText.getString().getSize() > mInputBehaviourArgument));
				if (EventManager::getPressedKeyInfo().key == sf::Keyboard::Key::BackSpace && inputBehaviourAllowsBackSpace)
				{
					std::string existingText = mText.getString();
					if (!existingText.empty())
					{
						existingText.pop_back();
					}
					mText.setString(existingText);
					this->setInternalObjectsPositions();
				}
			}
		}
	}
}

//Render
void TextField::render(sf::RenderWindow* renderWindow)
{
	renderWindow->draw(mRectangleShape);
	renderWindow->draw(mText);
	if (mTextFieldState == TextFieldState::BLINKING && mBlinkingLineVisible)
	{
		renderWindow->draw(*mBlinkingLineRect.pointer);
	}
}


//Setter
void TextField::setPosition(sf::Vector2f const & position)
{
	mPosition = position;
	this->setInternalObjectsPositions();
}
void TextField::move(sf::Vector2f const & move)
{
	mPosition += move;
	this->setInternalObjectsPositions();
}
void TextField::setSize(sf::Vector2f const & size)
{
	mSize = size;
	this->setInternalObjectsPositions();
}
void TextField::setTextFieldSettings(TextFieldSettings const * textFieldSettings)
{
	pTextFieldSettings = textFieldSettings;
	this->setInternalObjectsColors();
}
void TextField::setActive(bool active)
{
	if (active)
	{
		mTextFieldState = TextFieldState::NORMAL;
	}
	else
	{
		mTextFieldState = TextFieldState::INACTIVE;
	}
	this->setInternalObjectsColors();
}
void TextField::setTextString(std::string const & string)
{
	mText.setString(string);
	this->setInternalObjectsPositions();
}


//Getter
sf::Vector2f TextField::getPosition() const
{
	return mPosition;
}
sf::Vector2f TextField::getSize() const
{
	return mSize;
}
TextFieldSettings const * TextField::getTextFieldSettings() const
{
	return pTextFieldSettings;
}
bool TextField::getIsActive() const
{
	return (mTextFieldState != TextFieldState::INACTIVE);
}
TextFieldState TextField::getTextFieldState() const
{
	return mTextFieldState;
}
std::string TextField::getTextString() const
{
	return mText.getString();
}
unsigned int TextField::getTextStringSize() const
{
	return mText.getString().getSize();
}


//Internal Manipulators
void TextField::setInternalObjectsPositions()
{
	mRectangleShape.setPosition(mPosition);
	mText.setPosition(this->calculateTextPosition());
	mBlinkingLineRect.pointer->setPosition(this->calculateBlinkingLinePosition());
	mBlinkingLineRect.pointer->setSize(this->calculateBlinkingLineSize());

}
void TextField::setInternalObjectsColors()
{
	switch (mTextFieldState)
	{
	case TextFieldState::NORMAL:
		mRectangleShape.setFillColor(pTextFieldSettings->colorSettings.normalBackgroundColor);
		mRectangleShape.setOutlineColor(pTextFieldSettings->colorSettings.normalOutlineColor);
		mText.setFillColor(pTextFieldSettings->colorSettings.normalTextColor);
		break;
	case TextFieldState::BLINKING:
		mRectangleShape.setFillColor(pTextFieldSettings->colorSettings.blinkingBackgroundColor);
		mRectangleShape.setOutlineColor(pTextFieldSettings->colorSettings.blinkingOutlineColor);
		mText.setFillColor(pTextFieldSettings->colorSettings.blinkingTextColor);
		mBlinkingLineRect.setFillColor(pTextFieldSettings->colorSettings.blinkingTextColor);
		break;
	case TextFieldState::INACTIVE:
		mRectangleShape.setFillColor(pTextFieldSettings->colorSettings.inactiveBackgroundColor);
		mRectangleShape.setOutlineColor(pTextFieldSettings->colorSettings.inactiveOutlineColor);
		mText.setFillColor(pTextFieldSettings->colorSettings.inactiveTextColor);
		break;
	}
}

void TextField::changeStateTo(TextFieldState textFieldState)
{
	if (mTextFieldState != textFieldState)
	{
		mTextFieldState = textFieldState;
		this->setInternalObjectsColors();
		if (mTextFieldState == TextFieldState::BLINKING)
		{
			mBlinkingClock.restart();
			mBlinkingLineVisible = true;
		}
	}
}

sf::Vector2f TextField::calculateTextPosition() const
{
	sf::FloatRect globalBoundsText = mText.getGlobalBounds();
	sf::Vector2f sizeOfText = sf::Vector2f(globalBoundsText.width, globalBoundsText.height);
	sf::Vector2f relativePos(2.f, ((mSize - sizeOfText) / 2.f).y);
	sf::Vector2f correction = sf::Vector2f(0.f, -sizeOfText.y * 0.4f);
	return (mPosition + relativePos + correction);
}
sf::Vector2f TextField::calculateBlinkingLinePosition() const
{
	sf::FloatRect textRect = mText.getGlobalBounds();
	sf::Vector2f textRectPos = sf::Vector2f(textRect.left, textRect.top);
	sf::Vector2f textRectSize = sf::Vector2f(textRect.width, textRect.height);
	return (textRectPos + sf::Vector2f(textRectSize.x, 0.f));
}
sf::Vector2f TextField::calculateBlinkingLineSize() const
{
	return sf::Vector2f(2.f, static_cast<float>(mText.getCharacterSize()));
}

