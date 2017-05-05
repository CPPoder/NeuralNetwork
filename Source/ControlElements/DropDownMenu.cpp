#include "stdafx.h"
#include "Source\ControlElements\DropDownMenu.hpp"


DropDownMenu::DropDownMenu(sf::Vector2f const & position, sf::Vector2f const & sizeOfSingleField, sf::Vector2f const & sizeOfDropMenu, std::vector<std::string> const & vectorOfChoiceTexts, mySFML::Class::FontName fontName, bool active, DropDownMenuSettings const & dropDownMenuSettings)
	: pFont(mFonts.getFont(fontName)),
	  mDropDownMenuSettings(dropDownMenuSettings),
	  mPosition(position),
	  mSizeOfSingleField(sizeOfSingleField),
	  mSizeOfDropMenu(sizeOfDropMenu),
	  mRectShapeOfActualChoice(),
	  mTextDrawableOfActualChoice(*pFont, sf::Vector2f()),
	  mVectorOfChoiceTexts(vectorOfChoiceTexts),
	  mNumberOfChoices(vectorOfChoiceTexts.size()),
	  mRectShapeOfBarBackground(sf::Vector2f(), sf::Vector2f(), sf::Color::Blue, false, -2.f, sf::Color::Green),
	  mRectShapeOfBar(sf::Vector2f(), sf::Vector2f(), sf::Color::Red, false, -3.f, sf::Color::White)
{
	mTriangle.setPointCount(3u);
	mTriangle.setOutlineThickness(1.f);
	mTriangle.rotate(180.f);

	this->setActive(active);
	this->setInternalObjects();
}

DropDownMenu::~DropDownMenu()
{

}



void DropDownMenu::update(sf::Time const & frametime, sf::RenderWindow const * renderWindow)
{
	
	if (EventManager::checkForEvent(EventManager::EventType::KEY_RELEASED))
	{
		if (EventManager::getReleasedKeyInfo().key == sf::Keyboard::Key::Q)
		{
			this->setActive(!this->getIsActive());
		}
	}

	if (mIsActive)
	{
		//Manage Open/Close DropMenu & Selection of new currentChoice
		bool changeIsDropMenuOpen = false;
		if (EventManager::checkForEvent(EventManager::EventType::MOUSE_RELEASED))
		{
			if (EventManager::getReleasedMouseInfo().button == sf::Mouse::Button::Left)
			{
				sf::Vector2f mousePos = renderWindow->mapPixelToCoords(EventManager::getReleasedMouseInfo().position);

				//Open or Close
				if (mRectShapeOfActualChoice.pointer->getGlobalBounds().contains(mousePos))
				{
					changeIsDropMenuOpen = true;
				}
				else if (mIsDropDownMenuOpen && !mRectShapeOfBarBackground.pointer->getGlobalBounds().contains(mousePos) && !mRectShapeOfBar.pointer->getGlobalBounds().contains(mousePos))
				{
					changeIsDropMenuOpen = true;
				}

				//Select new currentChoice
				unsigned int rectNumber = 0;
				if (this->getChoiceRectFromMousePos(mousePos, rectNumber))
				{
					mCurrentChoice = rectNumber;
					changeIsDropMenuOpen = true;
				}
			} //Button is Left
		} //Mouse Released Event
		if (changeIsDropMenuOpen)
		{
			mIsDropDownMenuOpen = !mIsDropDownMenuOpen;
			this->setInternalObjects();
		}

		//Manage ColorChanges in normal or touched cases
		sf::Vector2f mousePos = renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*renderWindow));
		unsigned int rectNumber = 0;
		bool wasAnyTouched = mIsAnyChoiceTouched;
		unsigned int oldTouched = mTouchedChoice;
		if (this->getChoiceRectFromMousePos(mousePos, rectNumber))
		{
			mIsAnyChoiceTouched = true;
			mTouchedChoice = rectNumber;
		}
		else
		{
			mIsAnyChoiceTouched = false;
		}
		if ((wasAnyTouched != mIsAnyChoiceTouched) || (oldTouched != mTouchedChoice))
		{
			this->setInternalObjects();
		}

		//Manage Movement of the Bar
		if (mIsDropDownMenuOpen && EventManager::checkForEvent(EventManager::EventType::MOUSE_DRAGGED)) //Move Bar With Dragging
		{
			EventManager::MouseDraggedInfo draggedInfo = EventManager::getMouseDraggedInfo();
			if (draggedInfo.button == sf::Mouse::Button::Left)
			{
				sf::Vector2f oldMousePos = static_cast<sf::Vector2f>(draggedInfo.oldPosition);
				if (mRectShapeOfBarBackground.pointer->getGlobalBounds().contains(oldMousePos))
				{
					float barMovement = static_cast<float>(draggedInfo.newPosition.y) - oldMousePos.y;
					mCurrentPositionInDropMenu += barMovement;
					this->setInternalObjects();
				}
			}
		}
		if (mIsDropDownMenuOpen && EventManager::checkForEvent(EventManager::EventType::MOUSE_WHEEL_SCROLLED)) //Move Bar With Scrolling
		{
			EventManager::MouseWheelInfo wheelInfo = EventManager::getMouseWheelScrolledInfo();
			sf::Vector2f mousePos = static_cast<sf::Vector2f>(wheelInfo.position);
			if (this->checkIsMouseOverDropMenu(mousePos))
			{
				float constexpr scrollConstant = 8.f;
				mCurrentPositionInDropMenu -= scrollConstant * wheelInfo.delta;
				this->setInternalObjects();
			}
		}

	} //mIsActive
}

void DropDownMenu::render(sf::RenderWindow * renderWindow)
{
	//Draw Stuff in normal View
	renderWindow->draw(*mRectShapeOfActualChoice.pointer);
	renderWindow->draw(*mTextDrawableOfActualChoice.pointer);
	renderWindow->draw(mTriangle);
	if (mIsDropDownMenuOpen)
	{
		renderWindow->draw(*mRectShapeOfBarBackground.pointer);
		renderWindow->draw(*mRectShapeOfBar.pointer);
	}

	//Draw Stuff with special View
	sf::View initialView = renderWindow->getView();
	sf::View dropDownView = initialView;
	sf::Vector2f windowSize(static_cast<sf::Vector2f>(renderWindow->getSize()));
	sf::FloatRect wantedViewport(mPosition.x / windowSize.x, (mPosition.y + mSizeOfSingleField.y) / windowSize.y, mSizeOfDropMenu.x / windowSize.x, mSizeOfDropMenu.y / windowSize.y);
	dropDownView.setViewport(wantedViewport);
	dropDownView.setSize(mSizeOfDropMenu);
	dropDownView.setCenter(mPosition + sf::Vector2f(0.f, mSizeOfSingleField.y) + mSizeOfDropMenu / 2.f);
	renderWindow->setView(dropDownView);
	for (auto& rect : mVectorOfRectShapes)
	{
		renderWindow->draw(*rect.pointer);
	}
	for (auto& text : mVectorOfTexts)
	{
		renderWindow->draw(*text.pointer);
	}

	//Reset View
	renderWindow->setView(initialView);
}


void DropDownMenu::setPosition(sf::Vector2f const & position)
{
	mPosition = position;
	this->setInternalObjects();
}

void DropDownMenu::move(sf::Vector2f const & direction)
{
	mPosition += direction;
	this->setInternalObjects();
}

void DropDownMenu::setSizeOfSingleField(sf::Vector2f const & size)
{
	mSizeOfSingleField = size;
	this->setInternalObjects();
}

void DropDownMenu::setSizeOfDropMenu(sf::Vector2f const & size)
{
	mSizeOfDropMenu = size;
	this->setInternalObjects();
}

void DropDownMenu::setVectorOfChoiceTexts(std::vector<std::string> const & vectorOfTexts)
{
	mVectorOfChoiceTexts = vectorOfTexts;
	mNumberOfChoices = vectorOfTexts.size();
	this->setInternalObjects();
}

void DropDownMenu::addChoice(std::string const & text)
{
	mVectorOfChoiceTexts.push_back(text);
	mNumberOfChoices += 1;
	this->setInternalObjects();
}

void DropDownMenu::setActive(bool active)
{
	mIsActive = active;
	mIsDropDownMenuOpen = false;
	this->setInternalObjects();
}

void DropDownMenu::setCurrentChoice(unsigned int choiceNumber)
{
	mCurrentChoice = choiceNumber;
	this->setInternalObjects();
}

void DropDownMenu::setTextOfCurrentChoice(std::string const & text)
{
	mVectorOfChoiceTexts.at(mCurrentChoice) = text;
	this->setInternalObjects();
}

void DropDownMenu::setTextOfChoice(std::string const & text, unsigned int choiceNumber)
{
	mVectorOfChoiceTexts.at(choiceNumber) = text;
}



sf::Vector2f DropDownMenu::getPosition() const
{
	return mPosition;
}

sf::Vector2f DropDownMenu::getSizeOfSingleField() const
{
	return mSizeOfSingleField;
}

sf::Vector2f DropDownMenu::getSizeOfDropMenu() const
{
	return mSizeOfDropMenu;
}

bool DropDownMenu::getIsActive() const
{
	return mIsActive;
}

unsigned int DropDownMenu::getCurrentChoice() const
{
	return mCurrentChoice;
}

std::string DropDownMenu::getStringOfCurrentChoice() const
{
	return mVectorOfChoiceTexts.at(mCurrentChoice);
}

std::string DropDownMenu::getStringOfChoice(unsigned int choiceNumber) const
{
	return mVectorOfChoiceTexts.at(choiceNumber);
}



void DropDownMenu::setInternalObjects()
{
	//Reset all important Stuff
	mVectorOfRectShapes.clear();
	mVectorOfTexts.clear();

	//Check if mCurrentPositionInDropMenu is still valid
	if (mNumberOfChoices == 0u || (mNumberOfChoices * mSizeOfSingleField.y - mSizeOfDropMenu.y < 0.f))
	{
		mCurrentPositionInDropMenu = 0.f;
	}
	else
	{
		mCurrentPositionInDropMenu = myMath::Simple::trim(0.f, mCurrentPositionInDropMenu, mNumberOfChoices * mSizeOfSingleField.y - mSizeOfDropMenu.y); //(mNumberOfChoices - 1) * mSizeOfSingleField.y - mSizeOfDropMenu.y + mSizeOfSingleField.y = mNumberOfChoices * mSizeOfSingleField.y - mSizeOfDropMenu.y
	}
	

	//Set internal State describing variables
	mBarSizeRatio = myMath::Simple::min(mSizeOfDropMenu.y / (mNumberOfChoices * mSizeOfSingleField.y), 1.f);
	mBarPosRatio = mCurrentPositionInDropMenu / (mNumberOfChoices * mSizeOfSingleField.y);

	//Set Actually Chosen Rectangle & Text
	mRectShapeOfActualChoice = mySFML::Class::RectShape(this->constructActualChoiceRectPos(), mSizeOfSingleField, constructActualChoiceRectFillColor(), false, -2.f, constructActualChoiceRectOutlColor());
	mTextDrawableOfActualChoice = mySFML::Class::Text(*pFont, constructActualChoiceTextPos(), mVectorOfChoiceTexts.at(mCurrentChoice), mCharacterSize, constructActualChoiceTextColor());
	
	//Set Triangle
	mTriangle.setRadius(mSizeOfSingleField.y / 8.f);
	mTriangle.setOrigin(mTriangle.getRadius(), mTriangle.getRadius());
	mTriangle.setPosition(mPosition + mSizeOfSingleField + sf::Vector2f(-10.f, -mSizeOfSingleField.y / 2.f));
	mTriangle.setFillColor(this->constructTriangleBackgroundColor());
	mTriangle.setOutlineColor(this->constructTriangleOutlineColor());

	//Set all visible Choice Rectangles & Texts if mIsDropDownMenuOpen
	if (mIsDropDownMenuOpen)
	{
		sf::Vector2u visibilityBounds = this->getBoundsForVisibleRectangles();
		for (unsigned int i = visibilityBounds.x; i <= visibilityBounds.y; ++i)
		{
			mVectorOfRectShapes.push_back(mySFML::Class::RectShape(this->constructChoiceRectPos(i), mSizeOfSingleField, this->constructChoiceRectFillColor(i), false, -1.f, this->constructChoiceRectOutlColor(i)));
			mVectorOfTexts.push_back(mySFML::Class::Text(*pFont, this->constructChoiceTextPos(i), mVectorOfChoiceTexts.at(i), mCharacterSize, this->constructChoiceTextColor(i)));
		}
	}
	
	//Set BarBackground
	mRectShapeOfBarBackground.pointer->setPosition(mPosition + mSizeOfSingleField);
	mRectShapeOfBarBackground.pointer->setSize(sf::Vector2f(mBarWidth, mSizeOfDropMenu.y));
	mRectShapeOfBarBackground.pointer->setFillColor(this->constructBarBackgroundFillColor());
	mRectShapeOfBarBackground.pointer->setOutlineColor(this->constructBarBackgroundOutlineColor());

	//Set Bar
	sf::Vector2f barSize = sf::Vector2f(mBarWidth, mBarSizeRatio * mSizeOfDropMenu.y);
	mRectShapeOfBar.pointer->setSize(barSize);
	mRectShapeOfBar.pointer->setOrigin(barSize / 2.f);
	mRectShapeOfBar.pointer->setPosition(mPosition + mSizeOfSingleField + (barSize / 2.f) + sf::Vector2f(0.f, mBarPosRatio * mSizeOfDropMenu.y));
	mRectShapeOfBar.pointer->setFillColor(this->constructBarFillColor());
	mRectShapeOfBar.pointer->setOutlineColor(this->constructBarOutlineColor());

}



bool DropDownMenu::getChoiceRectFromMousePos(sf::Vector2f const & mousePos, unsigned int & choiceRect) const
{
	bool result = false;
	if (mIsDropDownMenuOpen)
	{
		bool xFits = (((mousePos.x - mPosition.x) < mSizeOfSingleField.x) && ((mousePos.x - mPosition.x) > 0.f));
		bool yFits = (((mousePos.y - mPosition.y - mSizeOfSingleField.y) < mSizeOfDropMenu.y) && ((mousePos.y - mPosition.y - mSizeOfSingleField.y) > 0.f));
		if (xFits && yFits)
		{
			int rectNumber = static_cast<int>((mousePos.y - mPosition.y - mSizeOfSingleField.y + mCurrentPositionInDropMenu) / mSizeOfSingleField.y);
			result = true;
			choiceRect = rectNumber;
		}
	}
	return result;
}




sf::Vector2f DropDownMenu::constructActualChoiceRectPos() const
{
	return mPosition;
}
sf::Color DropDownMenu::constructActualChoiceRectFillColor() const
{
	if (mIsActive)
	{
		return mDropDownMenuSettings.colorSettings.normalActualChoiceBackgroundColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.inactiveActualChoiceBackgroundColor;
	}
}
sf::Color DropDownMenu::constructActualChoiceRectOutlColor() const
{
	if (mIsActive)
	{
		return mDropDownMenuSettings.colorSettings.normalActualChoiceOutlineColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.inactiveActualChoiceOutlineColor;
	}
}

sf::Vector2f DropDownMenu::constructActualChoiceTextPos() const
{
	sf::Text text(mVectorOfChoiceTexts.at(mCurrentChoice), *pFont, mCharacterSize);
	sf::FloatRect textBounds = text.getGlobalBounds();
	sf::Vector2f textSize = sf::Vector2f(textBounds.width, textBounds.height);
	sf::Vector2f correction = sf::Vector2f(0.f, -textSize.y * 0.2f);
	return (mPosition + sf::Vector2f(10.f, (mSizeOfSingleField.y - textSize.y) / 2.f) + correction);
}
sf::Color DropDownMenu::constructActualChoiceTextColor() const
{
	if (mIsActive)
	{
		return mDropDownMenuSettings.colorSettings.normalActualChoiceTextColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.inactiveActualChoiceTextColor;
	}
}


sf::Vector2u DropDownMenu::getBoundsForVisibleRectangles() const
{
	if (mNumberOfChoices == 0u) //Ensure mNumberOfChoicese > 0u! Needed in Line 392, where 1 is subtracted!
	{
		return sf::Vector2u(0u, 0u);
	}
	unsigned int lowerBound = static_cast<unsigned int>(mCurrentPositionInDropMenu / mSizeOfSingleField.y);
	unsigned int upperBound = static_cast<unsigned int>((mCurrentPositionInDropMenu + mSizeOfDropMenu.y) / mSizeOfSingleField.y);
	return sf::Vector2u(myMath::Simple::trim(0u, lowerBound, mNumberOfChoices - 1), myMath::Simple::trim(0u, upperBound, mNumberOfChoices - 1));
}


sf::Vector2f DropDownMenu::constructChoiceRectPos(unsigned int choiceNumber) const
{
	return mPosition + sf::Vector2f(0.f, (choiceNumber + 1) * mSizeOfSingleField.y - mCurrentPositionInDropMenu);
}
sf::Color DropDownMenu::constructChoiceRectFillColor(unsigned int choiceNumber) const
{
	if (mIsAnyChoiceTouched && choiceNumber == mTouchedChoice)
	{
		return mDropDownMenuSettings.colorSettings.touchedDropDownBackgroundColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.normalDropDownBackgroundColor;
	}
}
sf::Color DropDownMenu::constructChoiceRectOutlColor(unsigned int choiceNumber) const
{
	if (mIsAnyChoiceTouched && choiceNumber == mTouchedChoice)
	{
		return mDropDownMenuSettings.colorSettings.touchedDropDownOutlineColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.normalDropDownOutlineColor;
	}
}

sf::Vector2f DropDownMenu::constructChoiceTextPos(unsigned int choiceNumber) const
{
	sf::Text text(mVectorOfChoiceTexts.at(choiceNumber), *pFont, mCharacterSize);
	sf::FloatRect textBounds = text.getGlobalBounds();
	sf::Vector2f textSize = sf::Vector2f(textBounds.width, textBounds.height);
	sf::Vector2f correction = sf::Vector2f(0.f, -textSize.y * 0.2f);
	return (this->constructChoiceRectPos(choiceNumber) + sf::Vector2f(10.f, (mSizeOfSingleField.y - textSize.y) / 2.f) + correction);
}
sf::Color DropDownMenu::constructChoiceTextColor(unsigned int choiceNumber) const
{
	if (mIsAnyChoiceTouched && choiceNumber == mTouchedChoice)
	{
		return mDropDownMenuSettings.colorSettings.touchedDropDownTextColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.normalDropDownTextColor;
	}
}


sf::Color DropDownMenu::constructTriangleBackgroundColor() const
{
	if (mIsDropDownMenuOpen)
	{
		return mDropDownMenuSettings.colorSettings.openTriangleBackgroundColor;
	}
	else if (mIsActive)
	{
		return mDropDownMenuSettings.colorSettings.closedTriangleBackgroundColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.inactiveTriangleBackgroundColor;
	}
}
sf::Color DropDownMenu::constructTriangleOutlineColor() const
{
	if (mIsDropDownMenuOpen)
	{
		return mDropDownMenuSettings.colorSettings.openTriangleOutlineColor;
	}
	else if (mIsActive)
	{
		return mDropDownMenuSettings.colorSettings.closedTriangleOutlineColor;
	}
	else
	{
		return mDropDownMenuSettings.colorSettings.inactiveTriangleOutlineColor;
	}
}



sf::Color DropDownMenu::constructBarBackgroundFillColor() const
{
	return mDropDownMenuSettings.colorSettings.barBackgroundFillColor;
}
sf::Color DropDownMenu::constructBarBackgroundOutlineColor() const
{
	return mDropDownMenuSettings.colorSettings.barBackgroundOutlineColor;
}
sf::Color DropDownMenu::constructBarFillColor() const
{
	return mDropDownMenuSettings.colorSettings.barFillColor;
}
sf::Color DropDownMenu::constructBarOutlineColor() const
{
	return mDropDownMenuSettings.colorSettings.barOutlineColor;
}


bool DropDownMenu::checkIsMouseOverDropMenu(sf::Vector2f const & mousePos) const
{
	if (mIsDropDownMenuOpen)
	{
		return (mySFML::Comp::smaller(mPosition + sf::Vector2f(0.f, mSizeOfSingleField.y), mousePos) && mySFML::Comp::smaller(mousePos, mPosition + sf::Vector2f(0.f, mSizeOfSingleField.y) + mSizeOfDropMenu));
	}
	else
	{
		return false;
	}
}


