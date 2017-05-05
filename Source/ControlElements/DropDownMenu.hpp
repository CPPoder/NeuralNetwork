#ifndef DROPDOWNMENU_HPP
#define DROPDOWNMENU_HPP


#include "SFML\Graphics.hpp"

#include "myUsefulMath.hpp"
#include "mySFMLVectorFunctions.hpp"

#include "Source\ControlElements\DropDownMenuSettings.hpp"
#include "Source\Framework\EventManager.hpp"

#include <vector>
#include <string>


class DropDownMenu
{
private:
	DropDownMenuSettings mDropDownMenuSettings;

	mySFML::Class::Fonts mFonts;
	sf::Font const * pFont;

	unsigned int mNumberOfChoices;
	std::vector<std::string> mVectorOfChoiceTexts;

	sf::Vector2f mPosition;
	sf::Vector2f mSizeOfSingleField;
	sf::Vector2f mSizeOfDropMenu;
	float mBarWidth = 20.f;

	unsigned int mCharacterSize = 12u;

	bool mIsActive = true;

	unsigned int mCurrentChoice = 0;
	float mCurrentPositionInDropMenu = 0.f;
	bool mIsDropDownMenuOpen = true;

	bool mIsAnyChoiceTouched = false;
	unsigned int mTouchedChoice = 0;

	float mBarSizeRatio;
	float mBarPosRatio;

	mySFML::Class::RectShape mRectShapeOfActualChoice;
	mySFML::Class::Text mTextDrawableOfActualChoice;
	sf::CircleShape mTriangle;

	std::vector<mySFML::Class::RectShape> mVectorOfRectShapes;
	std::vector<mySFML::Class::Text> mVectorOfTexts;

	mySFML::Class::RectShape mRectShapeOfBarBackground;
	mySFML::Class::RectShape mRectShapeOfBar;


public:
	DropDownMenu(sf::Vector2f const & position, sf::Vector2f const & sizeOfSingleField, sf::Vector2f const & sizeOfDropMenu, std::vector<std::string> const & vectorOfChoiceTexts, mySFML::Class::FontName fontName, bool active = true, DropDownMenuSettings const & dropDownMenuSettings = DropDownMenuSettings::standardDropDownMenuSettings);
	~DropDownMenu();

	void update(sf::Time const & frametime, sf::RenderWindow const * renderWindow);
	void render(sf::RenderWindow * renderWindow);


	void setPosition(sf::Vector2f const & position);
	void move(sf::Vector2f const & direction);
	void setSizeOfSingleField(sf::Vector2f const & size);
	void setSizeOfDropMenu(sf::Vector2f const & size);
	void setVectorOfChoiceTexts(std::vector<std::string> const & vectorOfTexts);
	void addChoice(std::string const & text);
	void setActive(bool active);
	void setCurrentChoice(unsigned int choiceNumber);
	void setTextOfCurrentChoice(std::string const & text);
	void setTextOfChoice(std::string const & text, unsigned int choiceNumber);

	sf::Vector2f getPosition() const;
	sf::Vector2f getSizeOfSingleField() const;
	sf::Vector2f getSizeOfDropMenu() const;
	bool getIsActive() const;
	unsigned int getCurrentChoice() const;
	std::string getStringOfCurrentChoice() const;
	std::string getStringOfChoice(unsigned int choiceNumber) const;


private:
	void setInternalObjects();
	
	bool getChoiceRectFromMousePos(sf::Vector2f const & mousePos, unsigned int & choiceRect) const;

	sf::Vector2f constructActualChoiceRectPos() const;
	sf::Color constructActualChoiceRectFillColor() const;
	sf::Color constructActualChoiceRectOutlColor() const;

	sf::Vector2f constructActualChoiceTextPos() const;
	sf::Color constructActualChoiceTextColor() const;

	sf::Vector2u getBoundsForVisibleRectangles() const;

	sf::Vector2f constructChoiceRectPos(unsigned int choiceNumber) const;
	sf::Color constructChoiceRectFillColor(unsigned int choiceNumber) const;
	sf::Color constructChoiceRectOutlColor(unsigned int choiceNumber) const;

	sf::Vector2f constructChoiceTextPos(unsigned int choiceNumber) const;
	sf::Color constructChoiceTextColor(unsigned int choiceNumber) const;

	sf::Color constructTriangleBackgroundColor() const;
	sf::Color constructTriangleOutlineColor() const;

	sf::Color constructBarBackgroundFillColor() const;
	sf::Color constructBarBackgroundOutlineColor() const;
	sf::Color constructBarFillColor() const;
	sf::Color constructBarOutlineColor() const;

public:
	bool checkIsMouseOverDropMenu(sf::Vector2f const & mousePos) const;


};





#endif //DROPDOWNMENU_HPP

