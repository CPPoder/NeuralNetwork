#include "stdafx.h"
#include "Source\MainModules\Editor.hpp"


Editor::Editor(sf::Vector2u const & editorWindowSize)
	: mEditorWindowSize(editorWindowSize),
	  mLoadTextField(mLoadTextFieldPos, mTextFieldSize, "./Data/Tracks/", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize, true, InputBehaviour::BOUNDED_FROM_BELOW, 14u),
	  mSaveTextField(mSaveTextFieldPos, mTextFieldSize, "./Data/Tracks/", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize, true, InputBehaviour::BOUNDED_FROM_BELOW, 14u),
	  mLoadButton(mLoadButtonPos, mButtonSize, "Load", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize),
	  mSaveButton(mSaveButtonPos, mButtonSize, "Save", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize),
	  mStatusTextField(mStatusTextFieldPos, mStatusTextFieldSize, "", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize, false, InputBehaviour::FREE, 0u, &TextFieldSettings::inactiveTextFieldSettings)
{
	this->setViews();

	mGUIBackgroundRectShape.setFillColor(sf::Color(80, 20, 50));
	mGUIBackgroundRectShape.setPosition(sf::Vector2f(0.f, 0.f));
	mGUIBackgroundRectShape.setSize(sf::Vector2f(mGUIRatio * mEditorWindowSize.x, static_cast<float>(mEditorWindowSize.y)));
}

Editor::~Editor()
{
	if (pCurrentTrack != nullptr)
	{
		delete pCurrentTrack;
		pCurrentTrack = nullptr;
	}
}


void Editor::render(sf::RenderWindow * renderWindow)
{
	sf::View originalView(renderWindow->getView());

	renderWindow->setView(mTrackView);
	this->renderTrack(renderWindow);

	renderWindow->setView(mGUIView);
	this->renderGUI(renderWindow);

	renderWindow->setView(originalView);
}
void Editor::renderGUI(sf::RenderWindow * renderWindow)
{
	renderWindow->draw(mGUIBackgroundRectShape);
	mLoadTextField.render(renderWindow);
	mSaveTextField.render(renderWindow);
	mLoadButton.render(renderWindow);
	mSaveButton.render(renderWindow);
	mStatusTextField.render(renderWindow);
}
void Editor::renderTrack(sf::RenderWindow * renderWindow)
{
	if (pCurrentTrack != nullptr)
	{
		pCurrentTrack->render(renderWindow, TrackRenderOptions::DEFAULT_FOR_EDITOR);
	}
}

void Editor::update(sf::Time const & time, sf::RenderWindow * renderWindow)
{
	//Update States
	mLoadTextField.updateState(renderWindow, &mGUIView);
	mSaveTextField.updateState(renderWindow, &mGUIView);
	mLoadButton.updateState(renderWindow, &mGUIView);
	mSaveButton.updateState(renderWindow, &mGUIView);
	mStatusTextField.updateState(renderWindow, &mGUIView);

	//Check for MouseReleasedEvents
	bool loadButtonReleased = mLoadButton.getMouseReleasedEventOccured(renderWindow, &mGUIView);
	bool saveButtonReleased = mSaveButton.getMouseReleasedEventOccured(renderWindow, &mGUIView);
	if (loadButtonReleased)
	{
		this->loadTrack(mLoadTextField.getTextString());
		if (!pCurrentTrack->getIfTrackIsWellInitialized())
		{
			this->discardTrack();
			this->showStatus("Loading failed!");
		}
		else
		{
			this->showStatus("Loading executed well!");
		}
		this->setViews();
	}
	if (saveButtonReleased)
	{
		if (pCurrentTrack != nullptr)
		{
			bool savingExecutedWell = pCurrentTrack->saveToFile(mSaveTextField.getTextString());
			if (savingExecutedWell)
			{
				this->showStatus("Saving executed well!");
			}
			else
			{
				this->showStatus("Saving failed!");
			}
		}
		else
		{
			this->showStatus("Saving failed! (No current Track!)");
		}
	}

	//Change Mode
	if (EventManager::checkForEvent(EventManager::EventType::KEY_RELEASED))
	{
		EventManager::KeyInfo keyInfo = EventManager::getReleasedKeyInfo();
		if (keyInfo.key == sf::Keyboard::Key::M)
		{
			switch (mMode)
			{
			case Mode::MODIFY_BORDER_TRACK_SEGMENTS:
				mMode = Mode::MODIFY_CENTER_TRACK_SEGMENTS;
				break;
			case Mode::MODIFY_CENTER_TRACK_SEGMENTS:
				mMode = Mode::MODIFY_BORDER_TRACK_SEGMENTS;
				break;
			}
		}
	}

	//Manipulate Track
	if (pCurrentTrack != nullptr)
	{
		this->manipulateTrack(renderWindow);
	}

	//Change Track View (Zoom, Movement)
	this->changeTrackView(renderWindow);
}


void Editor::loadTrack(std::string const & trackPath)
{
	if (pCurrentTrack != nullptr)
	{
		delete pCurrentTrack;
		pCurrentTrack = nullptr;
	}
	pCurrentTrack = new Track(trackPath);
}

void Editor::discardTrack()
{
	if (pCurrentTrack != nullptr)
	{
		delete pCurrentTrack;
		pCurrentTrack = nullptr;
	}
}


void Editor::showStatus(std::string const & status)
{
	mStatusTextField.setTextString(status);
}


Editor::Event Editor::pollEditorEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void Editor::reactOnESC()
{
	mEvent = Event::OPEN_EDITOR_MENU;
}


void Editor::setViews()
{
	sf::View newGUIView(sf::FloatRect(0.f, 0.f, mGUIRatio * mEditorWindowSize.x, mEditorWindowSize.y));
	newGUIView.setViewport(sf::FloatRect(0.f, 0.f, mGUIRatio, 1.f));
	mGUIView = newGUIView;

	float trackViewRatio = mEditorWindowSize.y / ((1.f - mGUIRatio) * mEditorWindowSize.x);
	float widthOfTrackView;
	if (pCurrentTrack != nullptr)
	{
		widthOfTrackView = pCurrentTrack->getValidTrackArea().width;
	}
	else
	{
		widthOfTrackView = static_cast<float>(mEditorWindowSize.x);
	}
	sf::View newTrackView(sf::FloatRect(0.f, 0.f, widthOfTrackView, trackViewRatio * widthOfTrackView));
	newTrackView.setViewport(sf::FloatRect(mGUIRatio, 0.f, (1.f - mGUIRatio), 1.f));
	mTrackView = newTrackView;
}


void Editor::changeTrackView(sf::RenderWindow const * renderWindow)
{
	//Scrolling
	if (EventManager::checkForEvent(EventManager::EventType::MOUSE_WHEEL_SCROLLED))
	{
		EventManager::MouseWheelInfo mouseWheelInfo = EventManager::getMouseWheelScrolledInfo();
		if (this->checkIfMouseIsInTrackViewport(mouseWheelInfo.position))
		{
			float constexpr scrollingConstant = 0.9f;
			float scroll = pow(scrollingConstant, mouseWheelInfo.delta);
			mTrackView.zoom(scroll);
		}
	}

	//Moving
	if (EventManager::checkForEvent(EventManager::EventType::MOUSE_DRAGGED))
	{
		EventManager::MouseDraggedInfo mouseDraggedInfo = EventManager::getMouseDraggedInfo();
		if (mouseDraggedInfo.button == sf::Mouse::Button::Middle)
		{
			if (this->checkIfMouseIsInTrackViewport(mouseDraggedInfo.oldPosition))
			{
				sf::Vector2f oldPos = renderWindow->mapPixelToCoords(mouseDraggedInfo.oldPosition, mTrackView);
				sf::Vector2f newPos = renderWindow->mapPixelToCoords(mouseDraggedInfo.newPosition, mTrackView);
				mTrackView.move(oldPos - newPos);
			}
		}
	}
}


bool Editor::checkIfMouseIsInTrackViewport(sf::Vector2i const & mousePos) const
{
	sf::FloatRect trackRectangle(mGUIRatio * mEditorWindowSize.x, 0.f, (1.f - mGUIRatio) * mEditorWindowSize.x, mEditorWindowSize.y);
	return trackRectangle.contains(static_cast<sf::Vector2f>(mousePos));
}



void Editor::manipulateTrack(sf::RenderWindow const * renderWindow)
{
	switch (mMode)
	{
	case Mode::MODIFY_BORDER_TRACK_SEGMENTS:
	case Mode::MODIFY_CENTER_TRACK_SEGMENTS:
		this->modifyBorderOrCenterTrackSegments(renderWindow);
		break;
	}
}


void Editor::modifyBorderOrCenterTrackSegments(sf::RenderWindow const * renderWindow)
{
	static bool mouseWasPressed = false;
	static bool circleWasHit = false;
	static sf::Vector2f mousePos;
	float constexpr circlesSensitiveRadius = 3.f;

	if (EventManager::checkForEvent(EventManager::EventType::MOUSE_PRESSED))
	{
		EventManager::MouseInfo mouseInfo = EventManager::getPressedMouseInfo();	
		if (mouseInfo.button == sf::Mouse::Button::Left)
		{
			mouseWasPressed = true;
			mousePos = renderWindow->mapPixelToCoords(mouseInfo.position, mTrackView);
			int dummyInt;
			if (pCurrentTrack->getIteratorToBorderTrackSegmentWhichHasBorder(mousePos, circlesSensitiveRadius, pCurrentTrack->getBorderTrackBase().begin(), dummyInt))
			{
				circleWasHit = true;
			}
		}
	}

	if (EventManager::checkForEvent(EventManager::EventType::MOUSE_RELEASED))
	{
		EventManager::MouseInfo mouseInfo = EventManager::getReleasedMouseInfo();
		if (mouseInfo.button == sf::Mouse::Button::Left)
		{
			mouseWasPressed = false;
			circleWasHit = false;
		}
	}

	if (mouseWasPressed && circleWasHit)
	{
		//Get new mousePos
		sf::Vector2f newMousePos = renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*renderWindow), mTrackView);
		sf::Vector2f mouseMovement = newMousePos - mousePos;

		//Find old circle and move
		BorderTrackBase::iterator it;
		int type;
		if (pCurrentTrack->getIteratorToBorderTrackSegmentWhichHasBorder(mousePos, circlesSensitiveRadius, it, type))
		{
			if (type == 1) //Handle Center Movements
			{
				it->first += mouseMovement;
				it->second += mouseMovement;
			}
			if (type == 2) //Handle first BorderMovements
			{
				if (mMode == Mode::MODIFY_BORDER_TRACK_SEGMENTS)
				{
					it->first += mouseMovement;
				}
				else if (mMode == Mode::MODIFY_CENTER_TRACK_SEGMENTS)
				{
					it->first += mouseMovement;
					it->second -= mouseMovement;
				}
			}
			if (type == 3) //Handle second BorderMovements
			{
				if (mMode == Mode::MODIFY_BORDER_TRACK_SEGMENTS)
				{
					it->second += mouseMovement;
				}
				else if (mMode == Mode::MODIFY_CENTER_TRACK_SEGMENTS)
				{
					it->second += mouseMovement;
					it->first -= mouseMovement;
				}
			}
			pCurrentTrack->refreshState();

			//Set mousePos = newMousePos
			mousePos = newMousePos;
		}
	}
}


