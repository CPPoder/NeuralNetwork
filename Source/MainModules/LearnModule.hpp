#ifndef LEARNMODULE_HPP
#define LEARNMODULE_HPP


#include "Source\MainModules\World.hpp"
#include "Source\Car\NetBrain.hpp"



class LearnModule
{
public:
	enum class Event
	{
		NONE,
		OPEN_LEARN_MODULE_MENU
	};

private:
	Event mEvent = Event::NONE;

private:
	NetBrain mBestNetBrain;
	NetBrain mCurrentNetBrain;
	float mCurrentIntegratedPath;
	float mBestRate;
	bool mFirstSample = true;
	sf::Time mSimulatedTimeOfCurrentSample;
	sf::Time mTimeWithoutMovement;

	World* pWorld = nullptr;

public:
	LearnModule();
	~LearnModule();

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

	Event pollLearnModuleEvent();
	void reactOnESC();


};








#endif //LEARNMODULE_HPP

