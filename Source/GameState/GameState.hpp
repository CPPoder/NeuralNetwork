#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "SFML\Graphics.hpp"


namespace GameState
{

	enum class GameStateChange
	{
		NO_CHANGE,

		PUSH_MAIN_MENU_STATE,
		PUSH_RACE_STATE,

		REPLACE_MAIN_MENU_STATE,
		REPLACE_RACE_STATE,

		POP,
		POP_TWICE,
		POP_TWICE_AND_PUSH_MAIN_MENU_STATE,
		POP_TWICE_AND_PUSH_RACE_STATE,


		NUM_OF_GAME_STATE_CHANGES
	};



	class GameState
	{
	public:
		virtual ~GameState() = default;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) = 0;
		virtual void render(sf::RenderWindow* renderWindow) = 0;
		virtual GameStateChange pollGameStateChange() = 0;
		virtual void reactOnESC() = 0;

	};

}



#endif //GAMESTATE_HPP

