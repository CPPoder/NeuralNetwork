#include "stdafx.h"
#include "Source\Framework\Framework.hpp"

//Constructor
Framework::Framework()
	: mFonts(),
	  mFPSText(*mFonts.getFont(mySFML::Class::FontName::ARIAL), sf::Vector2f(2.f, 0.f), "FPS: ", 12u, sf::Color::White),
	  mUtilizationText(*mFonts.getFont(mySFML::Class::FontName::ARIAL), sf::Vector2f(2.f, 10.f), "Util: ", 12u, sf::Color::White)
{
	srand(static_cast<unsigned int>(time(0)));

	pRenderWindow = new sf::RenderWindow(sf::VideoMode(800, 800), "Neural Network", sf::Style::Close | sf::Style::Titlebar);
	pRenderWindow->setFramerateLimit(60);

	mFPSClock.restart();

	TextureManager::Init();
	//SoundManager::Init();

	mStackOfGameStates.push(new GameState::MainMenuState);
}

//Destructor
Framework::~Framework()
{
	delete pRenderWindow;
	pRenderWindow = nullptr;

	while (!mStackOfGameStates.empty())
	{
		delete mStackOfGameStates.top();
		mStackOfGameStates.pop();
	}

	//SoundManager::DeInit();
}


//HandleEvents, Update, Render
void Framework::handleEvents()
{
	sf::Event mEvent;
	EventManager::reset();
	EventManager::checkForMouseDragging(pRenderWindow);
	while (pRenderWindow->pollEvent(mEvent))
	{
		//Handle immediately
		if (mEvent.type == sf::Event::EventType::Closed)
		{
			pRenderWindow->close();
		}
		if (mEvent.type == sf::Event::EventType::KeyReleased)
		{
			if (mEvent.key.code == sf::Keyboard::Key::Escape)
			{
				mStackOfGameStates.top()->reactOnESC();
			}
			if (mEvent.key.code == sf::Keyboard::Key::Space)
			{
				mShowFPS = !mShowFPS;
			}
		}

		//Pass the Events through to the EventManager
		if (mEvent.type == sf::Event::EventType::MouseButtonPressed)
		{
			EventManager::setPressedMouseEvent(EventManager::MouseInfo(mEvent.mouseButton.button, sf::Vector2i(mEvent.mouseButton.x, mEvent.mouseButton.y)));
		}
		if (mEvent.type == sf::Event::EventType::MouseButtonReleased)
		{
			EventManager::setReleasedMouseEvent(EventManager::MouseInfo(mEvent.mouseButton.button, sf::Vector2i(mEvent.mouseButton.x, mEvent.mouseButton.y)));
		}
		if (mEvent.type == sf::Event::EventType::MouseWheelScrolled)
		{
			if (mEvent.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
			{
				EventManager::setMouseWheelScrolledEvent(EventManager::MouseWheelInfo(mEvent.mouseWheelScroll.delta, sf::Vector2i(mEvent.mouseWheelScroll.x, mEvent.mouseWheelScroll.y)));
			}
		}
		if (mEvent.type == sf::Event::EventType::KeyPressed)
		{
			EventManager::setPressedKeyEvent(EventManager::KeyInfo(mEvent.key.code, mEvent.key.control, mEvent.key.alt, mEvent.key.shift, mEvent.key.system));
		}
		if (mEvent.type == sf::Event::EventType::KeyReleased)
		{
			EventManager::setReleasedKeyEvent(EventManager::KeyInfo(mEvent.key.code, mEvent.key.control, mEvent.key.alt, mEvent.key.shift, mEvent.key.system));
		}
		if (mEvent.type == sf::Event::EventType::TextEntered)
		{
			EventManager::setTextEnteredEvent(sf::String(mEvent.text.unicode));
		}

	}
}

void Framework::update()
{
	GameState::Change gameStateChange = mStackOfGameStates.top()->pollGameStateChange();

	GameState::GameState* gameStatePointer = nullptr;

	switch (gameStateChange.toState)
	{
	case GameState::Change::State::NO_STATE:
		break;
	case GameState::Change::State::MAIN_MENU:
		gameStatePointer = new GameState::MainMenuState;
		break;
	case GameState::Change::State::RACE:
		gameStatePointer = new GameState::RaceState;
		break;
	}

	switch (gameStateChange.type)
	{
	case GameState::Change::Type::NO_CHANGE:
		break;
	case GameState::Change::Type::POP:
		delete mStackOfGameStates.top();
		mStackOfGameStates.pop();
		break;
	case GameState::Change::Type::POP_TWICE:
		delete mStackOfGameStates.top();
		mStackOfGameStates.pop();
		delete mStackOfGameStates.top();
		mStackOfGameStates.pop();
		break;
	case GameState::Change::Type::PUSH:
		mStackOfGameStates.push(gameStatePointer);
		break;
	case GameState::Change::Type::REPLACE:
		delete mStackOfGameStates.top();
		mStackOfGameStates.pop();
		mStackOfGameStates.push(gameStatePointer);
		break;
	case GameState::Change::Type::POP_TWICE_AND_PUSH:
		delete mStackOfGameStates.top();
		mStackOfGameStates.pop();
		delete mStackOfGameStates.top();
		mStackOfGameStates.pop();
		mStackOfGameStates.push(gameStatePointer);
		break;
	}

	if (mStackOfGameStates.empty())
	{
		throw "Stack of GameStates is empty!";
	}

	mStackOfGameStates.top()->update(mFrametime, pRenderWindow);
}

void Framework::render()
{
	pRenderWindow->clear(sf::Color::Black);

	mStackOfGameStates.top()->render(pRenderWindow);
	if (mShowFPS)
	{
		pRenderWindow->draw(*mFPSText.pointer);
		pRenderWindow->draw(*mUtilizationText.pointer);
	}

	sf::Time timeBeforeRendering = mFPSClock.getElapsedTime();
	pRenderWindow->display();
	mRenderTimePerFrame = mFPSClock.getElapsedTime() - timeBeforeRendering;
}

//Determine FPS
void Framework::determineFrametime()
{
	static sf::Time timeSinceLastSecond(sf::seconds(0.f));
	static unsigned int framesSinceLastSecond(0u);
	static sf::Time renderTimeSinceLastSecond(sf::seconds(0.f));

	mFrametime = mFPSClock.restart();
	timeSinceLastSecond += mFrametime;
	renderTimeSinceLastSecond += mRenderTimePerFrame;
	++framesSinceLastSecond;

	if (timeSinceLastSecond > sf::seconds(1.f))
	{
		mFPS = framesSinceLastSecond;
        mFPSText.setString("FPS: " + std::to_string(mFPS));

		mUtilization = (timeSinceLastSecond - renderTimeSinceLastSecond).asSeconds();
		mUtilizationText.setString("Util: " + std::to_string(static_cast<int>(mUtilization * 100.f)) + "%");

		framesSinceLastSecond = 0u;
		timeSinceLastSecond = sf::seconds(0.f);
		renderTimeSinceLastSecond = sf::seconds(0.f);
	}

}


#define MEASURE_TIMES 0

//Run
void Framework::run()
{
	try
	{
#if MEASURE_TIMES
		sf::Clock measureTimesClock;
		measureTimesClock.restart();
		sf::Time detFTime;
		sf::Time handETime;
		sf::Time updTime;
		sf::Time rendTime;
#endif
		while (pRenderWindow->isOpen())
		{
#if MEASURE_TIMES
			measureTimesClock.restart();
#endif
			determineFrametime();
#if MEASURE_TIMES
			detFTime = measureTimesClock.restart();
#endif
			handleEvents();
#if MEASURE_TIMES
			handETime = measureTimesClock.restart();
#endif
			update();
#if MEASURE_TIMES
			updTime = measureTimesClock.restart();
#endif
			render();
#if MEASURE_TIMES
			rendTime = measureTimesClock.restart();
			std::cout << "Determine Frametime: " << detFTime.asMicroseconds() << " mms\n";
			std::cout << "Handle Events: " << handETime.asMicroseconds() << " mms\n";
			std::cout << "Update: " << updTime.asMicroseconds() << " mms\n";
			std::cout << "Render: " << rendTime.asMicroseconds() << " mms" << std::endl;
#endif
		}
	}
	catch (char* text) //Breaks the While-Loop, if the Stack of GameStates is empty!
	{
		pRenderWindow->close();
		std::cout << text << std::endl;
	}
}


//Get RenderWindow
sf::RenderWindow const * Framework::getRenderWindow()
{
	return pRenderWindow;
}

//Initialize pRenderWindow
sf::RenderWindow * Framework::pRenderWindow = nullptr;

