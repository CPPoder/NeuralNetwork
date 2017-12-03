#ifndef LEARNMODULE_HPP
#define LEARNMODULE_HPP


#include "Source\MainModules\World.hpp"


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
	//Some World stuff

public:
	LearnModule();
	~LearnModule();

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

	Event pollLearnModuleEvent();
	void reactOnESC();


};








#endif //LEARNMODULE_HPP

