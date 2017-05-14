#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "SFML\Graphics.hpp"

#include "Source\Framework\WindowChange.hpp"

#include <deque>


namespace GameState
{
	struct Change
	{
	public:
		enum class Type
		{
			NO_CHANGE,
			PUSH,
			REPLACE,
			POP,
			POP_TWICE,
			POP_TWICE_AND_PUSH
		} type;

		enum class State
		{
			NO_STATE,
			MAIN_MENU,
			RACE,
			RACE_MENU,
			EDITOR,
			EDITOR_MENU
		} toState;


	public:
		Change() = default;
		Change(Type _type, State _toState = State::NO_STATE);
		Change(Change const &) = default;
		Change& operator=(Change const &) = default;
		~Change() = default;

	};


	



	class GameState
	{
	public:
		virtual ~GameState() = default;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) = 0;
		virtual void render(sf::RenderWindow* renderWindow) = 0;
		virtual Change pollGameStateChange() = 0;
		virtual void reactOnESC() = 0;
		virtual std::deque<WindowChange> getWindowChanges() = 0;

	};

}



#endif //GAMESTATE_HPP

