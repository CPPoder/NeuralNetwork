#include "stdafx.h"
#include "Source\Animation\Animation.hpp"



//Constructors & Destructor
Animation::Animation(TextureManager::TextureName textureAtlasName, TextRectPositions const & textRectPositions, sf::Vector2f const & sizeOfTextRect, AnimProgram const & animProgram, AnimState initialAnimState, sf::Time const & animTime, sf::Vector2f const & position, bool centeredOrigin)
{
	pSprite = new sf::Sprite;

	this->setTextureAtlas(TextureManager::getTexture(textureAtlasName), textRectPositions, sizeOfTextRect, animProgram, initialAnimState, animTime);
	this->setPosition(position);
	if (centeredOrigin)
	{
		this->centerOrigin();
	}
}
Animation::~Animation()
{
	delete pSprite;
	pSprite = nullptr;
}

//Getter
sf::Color Animation::getColor() const
{
	return pSprite->getColor();
}
sf::FloatRect Animation::getGlobalBounds() const
{
	return pSprite->getGlobalBounds();
}
sf::Vector2f Animation::getOrigin() const
{
	return pSprite->getOrigin();
}
sf::Vector2f Animation::getPosition() const
{
	return pSprite->getPosition();
}
float Animation::getRotation() const
{
	return pSprite->getRotation();
}
sf::Vector2f Animation::getScale() const
{
	return pSprite->getScale();
}

//Setter
void Animation::move(sf::Vector2f const & movement)
{
	pSprite->move(movement);
}
void Animation::rotate(float rotation)
{
	pSprite->rotate(rotation);
}
void Animation::scale(sf::Vector2f const & scale)
{
	pSprite->scale(scale);
}
void Animation::setColor(sf::Color const & color)
{
	pSprite->setColor(color);
}
void Animation::setOrigin(sf::Vector2f const & origin)
{
	pSprite->setOrigin(origin);
}
void Animation::centerOrigin()
{
	this->setOrigin(mSizeOfTextureRect / 2.f);
}
void Animation::setPosition(sf::Vector2f const & position)
{
	pSprite->setPosition(position);
}
void Animation::setRotation(float rotation)
{
	pSprite->setRotation(rotation);
}
void Animation::setScale(sf::Vector2f const & scale)
{
	pSprite->setScale(scale);
}
void Animation::pauseAnimation()
{
	mPaused = true;
}
void Animation::continueAnimation()
{
	mPaused = false;
}

//Render & Update
void Animation::render(sf::RenderWindow* renderWindow) const
{
	renderWindow->draw(*pSprite);
}
void Animation::update(sf::Time const & frametime, sf::RenderWindow const * renderWindow)
{
	if (!mPaused)
	{
		mTimeSinceLastChange += frametime;
	}
	if (mTimeSinceLastChange > mAnimationTime)
	{
		mTimeSinceLastChange -= mAnimationTime;
		this->gotoNextAnimationState();
	}
}

//Animation Control
void Animation::setTextureAtlas(sf::Texture const * textureAtlas, TextRectPositions const & textRectPositions, sf::Vector2f const & sizeOfTextRect, AnimProgram const & animProgram, AnimState initialAnimState, sf::Time const & animTime)
{
	pTextureAtlas = textureAtlas;
	pSprite->setTexture(*pTextureAtlas);
	if (!textRectPositions.empty())
	{
		mTextRectPositions = textRectPositions;
	}
	else
	{
		std::cerr << "Animation::setTextureAtlas(..., TextRectPositions const & textRectPositions, ...): Error! textRectPositions is empty! Used trivial TextRectPositions!" << std::endl;
		mTextRectPositions = { sf::Vector2f() };
	}
	mSizeOfTextureRect = sizeOfTextRect;
	this->useAnimationProgram(animProgram, initialAnimState, true, animTime);
}
void Animation::useAnimationProgram(AnimProgram const & animProgram, AnimState initialAnimState, bool changeAnimTime, sf::Time const & animTime)
{
	if (!animProgram.empty())
	{
		mCurrentAnimProgram = animProgram;
	}
	else
	{
		std::cerr << "Animation::useAnimationProgram(AnimProgram const & animProgram, ...): Error! animProgram is empty! Used trivial Program!" << std::endl;
		mCurrentAnimProgram = { 0u };
	}
	bool foundInitialAnimState = false;
	for (AnimProgram::const_iterator it = mCurrentAnimProgram.cbegin(); it != mCurrentAnimProgram.cend(); ++it)
	{
		if (*it == initialAnimState)
		{
			mAnimationProgramState = it;
			foundInitialAnimState = true;
		}
	}
	if (!foundInitialAnimState)
	{
		std::cerr << "Animation::useAnimationProgram(..., AnimState initialAnimState, ...): Error! initialAnimState is not a valid state! Used first state in mCurrentAnimProgram!" << std::endl;
		mAnimationProgramState = mCurrentAnimProgram.cbegin();
	}
	if (changeAnimTime)
	{
		mAnimationTime = animTime;
	}
	this->setInternalTextureRect();
	mTimeSinceLastChange = sf::Time::Zero;
}


//Internal Functions
void Animation::gotoNextAnimationState()
{
	if (++mAnimationProgramState == mCurrentAnimProgram.cend())
	{
		mAnimationProgramState = mCurrentAnimProgram.cbegin();
	}
	this->setInternalTextureRect();
}

void Animation::setInternalTextureRect()
{
	sf::Vector2f textRectPos = mTextRectPositions.at(*mAnimationProgramState);
	pSprite->setTextureRect(static_cast<sf::IntRect>(sf::FloatRect(textRectPos.x, textRectPos.y, mSizeOfTextureRect.x, mSizeOfTextureRect.y)));
}
