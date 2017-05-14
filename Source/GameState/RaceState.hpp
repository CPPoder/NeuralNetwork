#ifndef RACESTATE_HPP
#define RACESTATE_HPP

#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\RaceSimulation.hpp"



namespace GameState
{

	class RaceState final : public GameState
	{
	private:
		RaceSimulation mRaceSimulation;
		bool mWindowAdjusted = false;

	public:
		RaceState();
		virtual ~RaceState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;
		virtual std::deque<WindowChange> getWindowChanges() override;

	};

}



#endif //RACESTATE_HPP

