#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "SFML\Graphics.hpp"

#include "Source\Track\Track.hpp"

#include "Source\ControlElements\Button.hpp"
#include "Source\ControlElements\CheckBox.hpp"
#include "Source\ControlElements\TextField.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include <iostream>
#include <string>


class Editor
{
public:
	//Define Event Enumeration Class (Handles Changes of GameStates)
	enum class Event
	{
		NONE,
		OPEN_EDITOR_MENU
	};

private:
	//Access to the Fonts
	mySFML::Class::Fonts mFonts;

	//The current Event (Will be polled by pollEditorEvent)
	Event mEvent = Event::NONE;

private:
	//General Editor Stuff (Views, Layout Ratios, ...)
	sf::Vector2u const mEditorWindowSize;
	float const mGUIRatio = 0.3f;
	sf::View mGUIView;
	sf::View mTrackView;

	//Track, which is currently loaded
	Track* pCurrentTrack = nullptr;

	//Menu Objects
	sf::RectangleShape mGUIBackgroundRectShape;

	sf::Vector2f const mButtonSize = sf::Vector2f(50.f, 25.f);
	sf::Vector2f const mLoadButtonPos = sf::Vector2f(100.f, 25.f);
	sf::Vector2f const mRelDistBetweenButtons = sf::Vector2f(0.f, 50.f);
	Button mLoadButton;
	Button mSaveButton;
	

public:
	//Constructors, Destructor & Assignment Operator
	Editor(sf::Vector2u const & editorWindowSize);
	~Editor();
	Editor(Editor const &) = delete;
	Editor& operator=(Editor const &) = delete;

	//Render & Update
	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

	//Track Loading (Also Manages Resources)
	void loadTrack(std::string const & trackPath);


	//Functions for external communication
	Event pollEditorEvent();
	void reactOnESC();


private:
	//Views & Rendering
	void setViews();
	void renderGUI(sf::RenderWindow * renderWindow);
	void renderTrack(sf::RenderWindow * renderWindow);

};






#endif //EDITOR_HPP

