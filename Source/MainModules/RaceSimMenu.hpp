#ifndef RACESIMMENU_HPP
#define RACESIMMENU_HPP

#include "SFML\Graphics.hpp"

#include "Source\ControlElements\Button.hpp"


class RaceSimMenu
{
public:
	enum class Event
	{
		NONE,
		CONTINUE,
		EXIT_RACE_SIMULATION
	};

private:
	Event mEvent = Event::NONE;

	mySFML::Class::Fonts mFonts;

	mySFML::Class::Text mTitleText;

	Button mContinueButton;
	Button mExitButton;
	

public:
	RaceSimMenu();
	~RaceSimMenu();
	RaceSimMenu(RaceSimMenu const &) = delete;
	RaceSimMenu& operator=(RaceSimMenu const &) = delete;

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

	Event pollRaceSimMenuEvent();
	void reactOnESC();

};






#endif //RACESIMMENU_HPP

