#ifndef LEARNSTATE_HPP
#define LEARNSTATE_HPP


#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\LearnModule.hpp"



namespace GameState
{

	class LearnState final : public GameState
	{
	private:
		LearnModule mLearnModule;
		bool mWindowAdjusted = false;

	public:
		LearnState();
		virtual ~LearnState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;
		virtual std::deque<WindowChange> getWindowChanges() override;

	};

}




#endif //LEARNSTATE_HPP

