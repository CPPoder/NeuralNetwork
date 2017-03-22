#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "Source\GameState\GameState.hpp"



namespace GameState
{

	class MainMenuState final : public GameState
	{
	private:
		bool mCloseMainMenu = false;

	public:
		virtual ~MainMenuState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual GameStateChange pollGameStateChange() override;
		virtual void reactOnESC() override;


	};

}



#endif //MAINMENUSTATE_HPP

