#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "SFML\Graphics.hpp"
#include "Source\Framework\TextureManager.hpp"
#include "Source\Framework\EventManager.hpp"
#include "Source\Animation\Animation.hpp"

#include <vector>


class CheckBox
{
private:
	bool mIsActive;
	bool mIsTicked;
	bool mHasChangedState;

	Animation mCheckBoxAnimation;

public:
	CheckBox(sf::Vector2f const & position, bool active = true, bool ticked = false, bool centeredOrigin = false, sf::Vector2f const & size = sf::Vector2f(16.f, 16.f));
	~CheckBox();

	void updateState(sf::RenderWindow* renderWindow, sf::View const * view = nullptr); //Pointer, beacause I need to have some default argument);
	void render(sf::RenderWindow* renderWindow);

	void setPosition(sf::Vector2f const & position);
	void move(sf::Vector2f const & move);
	void setSize(sf::Vector2f const & size);
	void setActive(bool active);
	void setTicked(bool ticked);

	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	bool getIsActive() const;
	bool getIsTicked() const;
	bool getHasChangedState();


private:
	void changeStateTo(bool active, bool ticked, bool changeStateGuaranteed = false);

};


#endif //CHECKBOX_HPP

