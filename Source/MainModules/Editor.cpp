#include "stdafx.h"
#include "Source\MainModules\Editor.hpp"


Editor::Editor(sf::Vector2u const & editorWindowSize)
	: mEditorWindowSize(editorWindowSize),
	  mLoadTextField(mLoadTextFieldPos, mTextFieldSize, "./Data/Tracks/", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize, true, InputBehaviour::BOUNDED_FROM_BELOW, 14u),
	  mSaveTextField(mSaveTextFieldPos, mTextFieldSize, "./Data/Tracks/", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize, true, InputBehaviour::BOUNDED_FROM_BELOW, 14u),
	  mLoadButton(mLoadButtonPos, mButtonSize, "Load", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize),
	  mSaveButton(mSaveButtonPos, mButtonSize, "Save", mySFML::Class::FontName::ARIAL, 2.f, mCharacterSize)
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
}
void Editor::renderTrack(sf::RenderWindow * renderWindow)
{

}

void Editor::update(sf::Time const & time, sf::RenderWindow * renderWindow)
{
	mLoadTextField.updateState(renderWindow, &mGUIView);
	mSaveTextField.updateState(renderWindow, &mGUIView);
	mLoadButton.updateState(renderWindow, &mGUIView);
	mSaveButton.updateState(renderWindow, &mGUIView);
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

	sf::View newTrackView(sf::FloatRect(mGUIRatio * mEditorWindowSize.x, 0.f, (1.f - mGUIRatio) * mEditorWindowSize.x, mEditorWindowSize.y));
	newTrackView.setViewport(sf::FloatRect(mGUIRatio, 0.f, (1.f - mGUIRatio), 1.f));
	mTrackView = newTrackView;
}
