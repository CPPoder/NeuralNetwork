#ifndef LEARNMENUSTATE_HPP
#define LEARNMENUSTATE_HPP



#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\LearnModuleMenu.hpp"



namespace GameState
{

	class LearnMenuState final : public GameState
	{
	private:
		LearnModuleMenu mLearnModuleMenu;

	public:
		LearnMenuState();
		virtual ~LearnMenuState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;
		virtual std::deque<WindowChange> getWindowChanges() override;

	};

}






#endif //LEARNMENUSTATE_HPP

