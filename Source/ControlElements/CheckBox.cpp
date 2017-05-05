#include "stdafx.h"
#include "Source\ControlElements\CheckBox.hpp"

//Constructor
CheckBox::CheckBox(sf::Vector2f const & position, bool active, bool ticked, bool centeredOrigin, sf::Vector2f const & size)
	: mCheckBoxAnimation(TextureManager::TextureName::CHECK_BOX_TEXTURE_ATLAS, { { 0.f, 0.f },{ 16.f, 0.f },{ 0.f, 16.f },{ 16.f, 16.f } }, sf::Vector2f(16.f, 16.f), { 0u }, 0u, sf::seconds(1000.f), position, centeredOrigin)
{
	this->changeStateTo(active, ticked, true);
	this->setSize(size);
}

//Destructor
CheckBox::~CheckBox()
{
}


//UpdateState
void CheckBox::updateState(sf::RenderWindow* renderWindow, sf::View const * view)
{
	if (mIsActive && EventManager::checkForEvent(EventManager::EventType::MOUSE_RELEASED))
	{
		EventManager::MouseInfo mouseInfo = EventManager::getReleasedMouseInfo();
		if (mouseInfo.button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mousePos;
			if (view == nullptr)
			{
				mousePos = renderWindow->mapPixelToCoords(mouseInfo.position);
			}
			else
			{
				mousePos = renderWindow->mapPixelToCoords(mouseInfo.position, *view);
			}
			if (mCheckBoxAnimation.getGlobalBounds().contains(mousePos))
			{
				this->changeStateTo(true, !this->getIsTicked());
			}
		}
	}
}

//Render
void CheckBox::render(sf::RenderWindow* renderWindow)
{
	mCheckBoxAnimation.render(renderWindow);
}


//Setter
void CheckBox::setPosition(sf::Vector2f const & position)
{
	mCheckBoxAnimation.setPosition(position);
}
void CheckBox::move(sf::Vector2f const & move)
{
	mCheckBoxAnimation.move(move);
}
void CheckBox::setSize(sf::Vector2f const & size)
{
	mCheckBoxAnimation.setScale(sf::Vector2f(size.x / 16.f, size.y / 16.f));
}
void CheckBox::setActive(bool active)
{
	this->changeStateTo(active, this->getIsTicked());
}
void CheckBox::setTicked(bool ticked)
{
	this->changeStateTo(this->getIsActive(), ticked);
}


//Getter
sf::Vector2f CheckBox::getPosition() const
{
	return mCheckBoxAnimation.getPosition();
}
sf::Vector2f CheckBox::getSize() const
{
	sf::Vector2f scale = mCheckBoxAnimation.getScale();
	return sf::Vector2f(16.f * scale.x, 16.f * scale.y);
}
bool CheckBox::getIsActive() const
{
	return mIsActive;
}
bool CheckBox::getIsTicked() const
{
	return mIsTicked;
}
bool CheckBox::getHasChangedState()
{
	bool stateChanged = mHasChangedState;
	mHasChangedState = false;
	return stateChanged;
}




//Internal Functions
void CheckBox::changeStateTo(bool active, bool ticked, bool changeStateGuaranteed)
{
	if (mIsActive != active || mIsTicked != ticked || changeStateGuaranteed)
	{
		mIsActive = active;
		mIsTicked = ticked;
		mHasChangedState = true;
		unsigned int program;
		if (active && ticked)
		{
			program = 2u;
		}
		else if (active && !ticked)
		{
			program = 0u;
		}
		else if (!active && ticked)
		{
			program = 3u;
		}
		else if (!active && !ticked)
		{
			program = 1u;
		}
		mCheckBoxAnimation.useAnimationProgram({ program }, program);
		mCheckBoxAnimation.pauseAnimation();
	}
}


