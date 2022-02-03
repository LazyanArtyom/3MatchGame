#ifndef MATCH3_GAMEOVERSTATE_HPP_
#define MATCH3_GAMEOVERSTATE_HPP_

#include "GameController.hpp"
#include "State.hpp"

#include <SFML/Graphics.hpp>

namespace match3 {
	/**
	* \brief GameOverState end of the game.
	*
	* Creates at the end of the game.
	* If objectives completed then you win, if not, you lose.
	*/
	class GameOverState final : public impl::IState
	{
	public:
		GameOverState(GameController::GameDataPtrType data, bool isWon);

		// Inherited via IState
		void init() override;
		void handleInput() override;
		void draw() override;
		void update() override;

	private:
		bool isWon_;
		sf::Text title_;
		sf::Sprite background_;
		sf::Sprite retryButton_;

		GameController::GameDataPtrType data_;
	};

} // end namespace match3
#endif // MATCH3_GAMEOVERSTATE_HPP_
