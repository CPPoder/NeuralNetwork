#include "stdafx.h"
#include "Source\MainModules\LearnModule.hpp"

LearnModule::LearnModule()
{

}

LearnModule::~LearnModule()
{
}


void LearnModule::render(sf::RenderWindow * renderWindow)
{
	//Render occasionally
}

void LearnModule::update(sf::Time const & time, sf::RenderWindow const * renderWindow)
{
	//Learn!!!
}




LearnModule::Event LearnModule::pollLearnModuleEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void LearnModule::reactOnESC()
{
	mEvent = Event::OPEN_LEARN_MODULE_MENU;
}
