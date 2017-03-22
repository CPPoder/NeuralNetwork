#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "SFML\Graphics.hpp"

#include <iostream>
#include <string>


class EventManager
{
public:
	enum class EventType
	{
		MOUSE_PRESSED,
		MOUSE_RELEASED,
		KEY_PRESSED,
		KEY_RELEASED,
		TEXT_ENTERED,
		MOUSE_WHEEL_SCROLLED,
		MOUSE_DRAGGED,

		NUM_OF_EVENT_TYPES
	};

	enum class TextMode
	{
		UNFILTERED,
		FILTER_FOR_ASCII,
		FILTER_FOR_PRINTABLE_ASCII,

		NUM_OF_TEXT_MODES
	};

	struct MouseInfo
	{
		sf::Mouse::Button button;
		sf::Vector2i position;

		MouseInfo(sf::Mouse::Button const & _button, sf::Vector2i const & _position)
			: button(_button), position(_position)
		{
		}
	};

	struct MouseWheelInfo
	{
		float delta;
		sf::Vector2i position;

		MouseWheelInfo(float _delta, sf::Vector2i const & _position)
			: delta(_delta), position(_position)
		{
		}
	};

	struct KeyInfo
	{
		sf::Keyboard::Key key;
		bool control;
		bool alt;
		bool shift;
		bool system;

		KeyInfo(sf::Keyboard::Key const & _key, bool _control, bool _alt, bool _shift, bool _system)
			: key(_key), control(_control), alt(_alt), shift(_shift), system(_system)
		{
		}
	};

	struct MouseDraggedInfo
	{
		sf::Mouse::Button button;
		sf::Vector2i oldPosition;
		sf::Vector2i newPosition;

		MouseDraggedInfo(sf::Mouse::Button const & _button, sf::Vector2i const & _oldPosition, sf::Vector2i const & _newPosition)
			: button(_button), oldPosition(_oldPosition), newPosition(_newPosition)
		{
		}
	};


private:
	//Mouse Pressed
	static bool					mIsMousePressed;
	static sf::Mouse::Button	mPressedMouseButton;
	static sf::Vector2i			mPressedMousePosition;

	//Mouse Released
	static bool					mIsMouseReleased;
	static sf::Mouse::Button	mReleasedMouseButton;
	static sf::Vector2i			mReleasedMousePosition;

	//Mouse Wheel Scrolled
	static bool					mIsMouseWheelScrolled;
	static float				mMouseWheelScrolledDelta;
	static sf::Vector2i			MMouseWheelScrolledPosition;

	//KeyPressed
	static bool					mIsKeyPressed;
	static sf::Keyboard::Key	mPressedKey;
	static bool					mPressedKeyWithControl;
	static bool					mPressedKeyWithAlt;
	static bool					mPressedKeyWithShift;
	static bool					mPressedKeyWithSystem;

	//KeyReleased
	static bool					mIsKeyReleased;
	static sf::Keyboard::Key	mReleasedKey;
	static bool					mReleasedKeyWithControl;
	static bool					mReleasedKeyWithAlt;
	static bool					mReleasedKeyWithShift;
	static bool					mReleasedKeyWithSystem;

	//TextEntered
	static bool					mIsTextEntered;
	static sf::String			mText;

	//MouseDragged
	static bool					mDraggedWasMousePressed;
	static bool					mDraggedIsMousePressed;
	static sf::Mouse::Button	mDraggedOldMouseButton;
	static sf::Mouse::Button	mDraggedNewMouseButton;
	static bool					mIsMouseDragged;
	static sf::Mouse::Button	mDraggedButton;
	static sf::Vector2i			mDraggedOldMousePos;
	static sf::Vector2i			mDraggedNewMousePos;


public:
	//Delete Constructors, since EventManager is not meant to be instantiated!
	EventManager() = delete;
	~EventManager() = delete;
	EventManager(EventManager const &) = delete;
	EventManager& operator=(EventManager const &) = delete;


	////////
	//Getter

	static bool checkForEvent(EventType eventType);
	static MouseInfo getPressedMouseInfo();
	static MouseInfo getReleasedMouseInfo();
	static MouseWheelInfo getMouseWheelScrolledInfo();
	static KeyInfo getPressedKeyInfo();
	static KeyInfo getReleasedKeyInfo();
	static std::string getTextEnteredText(TextMode textMode = TextMode::UNFILTERED);
	static MouseDraggedInfo getMouseDraggedInfo();


	////////
	//Setter

	static void reset();
	static void setPressedMouseEvent(MouseInfo mouseInfo);
	static void setReleasedMouseEvent(MouseInfo mouseInfo);
	static void setMouseWheelScrolledEvent(MouseWheelInfo mouseWheelInfo);
	static void setPressedKeyEvent(KeyInfo keyInfo);
	static void setReleasedKeyEvent(KeyInfo keyInfo);
	static void setTextEnteredEvent(sf::String const & text);
	static void checkForMouseDragging(sf::RenderWindow const * renderWindow);



};




#endif //EVENTMANAGER_HPP

