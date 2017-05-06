#ifndef EDITORMENU_HPP
#define EDITORMENU_HPP

#include "SFML\Graphics.hpp"

#include "Source\ControlElements\Button.hpp"


class EditorMenu
{
public:
	enum class Event
	{
		NONE,
		CONTINUE,
		EXIT_EDITOR
	};

private:
	Event mEvent = Event::NONE;

	mySFML::Class::Fonts mFonts;

	mySFML::Class::Text mTitleText;

	Button mContinueButton;
	Button mExitButton;
	

public:
	EditorMenu();
	~EditorMenu();
	EditorMenu(EditorMenu const &) = delete;
	EditorMenu& operator=(EditorMenu const &) = delete;

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

	Event pollEditorMenuEvent();
	void reactOnESC();

};






#endif //EDITORMENU_HPP

