#ifndef MATCH3_MAINMENUSTATE_HPP_
#define MATCH3_MAINMENUSTATE_HPP_

#include "GameController.hpp"
#include "State.hpp"

#include <SFML/Graphics.hpp>

namespace match3 {
	/**
	* \brief MainMenuState represents main menu at game start
	*/
	class MainMenuState final : public impl::IState
	{
	public:
		MainMenuState(GameController::GameDataPtrType data);

		// Inherited via IState
		void init() override;
		void handleInput() override;
		void draw() override;
		void update() override;

	private:
		sf::Sprite title_;
		sf::Sprite background_;
		sf::Sprite playButton_;

		GameController::GameDataPtrType data_;
	};

} // end namespace match3
#endif // MATCH3_MAINMENUSTATE_HPP_