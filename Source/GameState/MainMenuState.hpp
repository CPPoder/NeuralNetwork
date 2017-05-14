#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\MainMenu.hpp"



namespace GameState
{

	class MainMenuState final : public GameState
	{
	private:
		MainMenu mMainMenu;
		bool mWindowAdjusted = false;

	public:
		MainMenuState();
		virtual ~MainMenuState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;
		virtual std::deque<WindowChange> getWindowChanges() override;

	};

}



#endif //MAINMENUSTATE_HPP

