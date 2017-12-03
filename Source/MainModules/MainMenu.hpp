#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "SFML\Graphics.hpp"

#include "Source\ControlElements\Button.hpp"


class MainMenu
{
public:
	enum class Event
	{
		NONE,
		EXIT,
		START_SIMULATION,
		OPEN_EDITOR,
		OPEN_LEARNING
	};

private:
	Event mEvent = Event::NONE;

	mySFML::Class::Fonts mFonts;

	mySFML::Class::Text mTitleText;

	Button mStartSimulationButton;
	Button mStartLearnButton;
	Button mStartEditorButton;
	Button mExitButton;
	

public:
	MainMenu();
	~MainMenu();
	MainMenu(MainMenu const &) = delete;
	MainMenu& operator=(MainMenu const &) = delete;

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

	Event pollMainMenuEvent();
	void reactOnESC();

};






#endif //MAINMENU_HPP

