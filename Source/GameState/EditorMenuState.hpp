#ifndef EDITORMENUSTATE_HPP
#define EDITORMENUSTATE_HPP

#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\EditorMenu.hpp"



namespace GameState
{

	class EditorMenuState final : public GameState
	{
	private:
		EditorMenu mEditorMenu;

	public:
		EditorMenuState();
		virtual ~EditorMenuState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;
		virtual std::deque<WindowChange> getWindowChanges() override;


	};

}



#endif //EDITORMENUSTATE_HPP

