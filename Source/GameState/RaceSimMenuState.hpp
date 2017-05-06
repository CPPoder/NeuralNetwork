#ifndef RACESIMMENUSTATE_HPP
#define RACESIMMENUSTATE_HPP

#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\RaceSimMenu.hpp"



namespace GameState
{

	class RaceSimMenuState final : public GameState
	{
	private:
		RaceSimMenu mRaceSimMenu;

	public:
		RaceSimMenuState();
		virtual ~RaceSimMenuState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;


	};

}



#endif //RACESIMMENUSTATE_HPP

