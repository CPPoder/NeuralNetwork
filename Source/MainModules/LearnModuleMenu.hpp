#ifndef LEARNMODULEMENU_HPP
#define LEARNMODULEMENU_HPP


#include "SFML\Graphics.hpp"

#include "Source\ControlElements\Button.hpp"



class LearnModuleMenu
{
public:
	enum class Event
	{
		NONE,
		CONTINUE,
		EXIT_LEARN_MODULE
	};

private:
	Event mEvent = Event::NONE;

	mySFML::Class::Fonts mFonts;

	mySFML::Class::Text mTitleText;

	Button mContinueButton;
	Button mExitButton;


public:
	LearnModuleMenu();
	~LearnModuleMenu();
	LearnModuleMenu(LearnModuleMenu const &) = delete;
	LearnModuleMenu& operator=(LearnModuleMenu const &) = delete;

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

	Event pollLearnModuleMenuEvent();
	void reactOnESC();

};







#endif //LEARNMODULEMENU_HPP

