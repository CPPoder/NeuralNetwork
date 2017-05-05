#ifndef RACESTATE_HPP
#define RACESTATE_HPP

#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\RaceSimulation.hpp"



namespace GameState
{

	class RaceState final : public GameState
	{
	private:
		bool mCloseRaceState = false;
		RaceSimulation mRaceSimulation;

	public:
		virtual ~RaceState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;


	};

}



#endif //MAINMENUSTATE_HPP

