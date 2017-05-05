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
		START_SIMULATION
	};

private:
	Event mEvent = Event::NONE;
	Button mStartSimulationButton;
	Button mExitButton;
	

public:
	MainMenu();
	~MainMenu();
	MainMenu(MainMenu const &) = delete;
	MainMenu& operator=(MainMenu const &) = delete;

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

	Event getMainMenuEvent() const;
	void reactOnESC();

};






#endif //MAINMENU_HPP

