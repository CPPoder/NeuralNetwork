#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "SFML\Graphics.hpp"


class Editor
{
public:
	enum class Event
	{
		NONE,
		OPEN_EDITOR_MENU
	};

private:
	Event mEvent = Event::NONE;
	

public:
	Editor();
	~Editor();
	Editor(Editor const &) = delete;
	Editor& operator=(Editor const &) = delete;

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

	Event pollEditorEvent();
	void reactOnESC();

};






#endif //EDITOR_HPP

