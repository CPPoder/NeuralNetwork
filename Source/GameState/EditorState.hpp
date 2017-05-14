#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include "Source\GameState\GameState.hpp"
#include "Source\MainModules\Editor.hpp"



namespace GameState
{

	class EditorState final : public GameState
	{
	private:
		Editor mEditor;
		bool mWindowAdjusted = false;

	public:
		virtual ~EditorState() override;

		virtual void update(sf::Time const & frametime, sf::RenderWindow* renderWindow) override;
		virtual void render(sf::RenderWindow* renderWindow) override;
		virtual Change pollGameStateChange() override;
		virtual void reactOnESC() override;
		virtual std::deque<WindowChange> getWindowChanges() override;

	};

}



#endif //EDITORSTATE_HPP

