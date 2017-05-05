#ifndef ANIMATION_HPP
#define ANIMATION_HPP


#include "SFML\Graphics.hpp"

#include "Source\Framework\TextureManager.hpp"

#include <iostream>
#include <vector>
#include <list>





class Animation
{
public:
	typedef std::vector<sf::Vector2f> TextRectPositions;
	typedef unsigned int AnimState;
	typedef std::list<AnimState> AnimProgram;


private:
	sf::Texture const * pTextureAtlas;

	sf::Sprite* pSprite;

	sf::Vector2f mSizeOfTextureRect;

	TextRectPositions mTextRectPositions;
	AnimProgram mCurrentAnimProgram;
	AnimProgram::const_iterator mAnimationProgramState;
	sf::Time mAnimationTime;

	bool mPaused = false;

	sf::Time mTimeSinceLastChange = sf::seconds(0.f);

public:
	//Constructors & Destructor
	Animation(TextureManager::TextureName textureAtlasName, TextRectPositions const & textRectPositions, sf::Vector2f const & sizeOfTextRect, AnimProgram const & animProgram, AnimState initialAnimState, sf::Time const & animTime, sf::Vector2f const & position, bool centeredOrigin = true);
	Animation(Animation const &) = delete;
	Animation& operator=(Animation const &) = delete;
	
	~Animation();

	//Getter
	sf::Color getColor() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Vector2f getOrigin() const;
	sf::Vector2f getPosition() const;
	float getRotation() const;
	sf::Vector2f getScale() const;

	//Setter
	void move(sf::Vector2f const & movement);
	void rotate(float rotation);
	void scale(sf::Vector2f const & scale);
	void setColor(sf::Color const & color);
	void setOrigin(sf::Vector2f const & origin);
	void centerOrigin();
	void setPosition(sf::Vector2f const & position);
	void setRotation(float rotation);
	void setScale(sf::Vector2f const & scale);
	void pauseAnimation();
	void continueAnimation();

	//Render & Update
	void render(sf::RenderWindow* renderWindow) const;
	void update(sf::Time const & frametime, sf::RenderWindow const * renderWindow);

	//Animation Control
	void setTextureAtlas(sf::Texture const * textureAtlas, TextRectPositions const & textRectPositions, sf::Vector2f const & sizeOfTextRect, AnimProgram const & animProgram, AnimState initialAnimState, sf::Time const & animTime);
	void useAnimationProgram(AnimProgram const & animProgram, AnimState initialAnimState, bool changeAnimTime = false, sf::Time const & animTime = sf::seconds(1.f));


private:
	void gotoNextAnimationState();
	void setInternalTextureRect();

};






#endif //ANIMATION_HPP

