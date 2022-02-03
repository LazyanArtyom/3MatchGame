#ifndef MATCH3_GAMESTATE_HPP_
#define MATCH3_GAMESTATE_HPP_

#include "Board.hpp"
#include "State.hpp"
#include "Figure.hpp"
#include "GameController.hpp"

#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace match3 {
	/**
	* \brief GameState is a main game logic
	*/
	class GameState final : public impl::IState
	{
	public:
		enum class Status : int
		{
			WAITING, SELECTED, SWAPPING, MOVING, ARRANGING, LOSING, WINNING
		};

		enum class MatchType : int
		{
			HORIZONTAL, VERTICAL, RADIAL, NONE
		};

		struct Match
		{
			int count = 0;
			MatchType type;
			Figure::Color color;
		};

		using BoardPtrType = std::unique_ptr<Board>;

		// match type and pattern
		using MatchPatternCollectionType = 
			std::unordered_map<MatchType, std::function<int(int, int, std::vector<Figure>::iterator&)>>;

		GameState(GameController::GameDataPtrType data);

		// Inherited via IState
		void init() override;
		void handleInput() override;
		void draw() override;
		void update() override;

		void reset();
		void resetFigures();
		void resetObjectives();
		void resetMovesCount();

		void setStatus(Status status);
		bool checkStatus(Status status) const;
		Figure::Color getRandomColor() const;

		void click(const sf::Vector2f& pos);

		void decrementMove();
		int getMovesCount() const;

		bool arrange();
		void replaceDeleted();

		void match();
		void initMatchPatterns();
		int matchColor(std::vector<Figure*> figures);

		void explode(Figure* figure);
		void spawnBomb(Figure::Color color);

		void checkEndGame();
		void checkForBombs();
		void checkObjectives();

	private:
		int colorsCount_ = 5;
		bool isFirstArrange_ = true;

		sf::Text movesCount_;
		sf::Sprite background_;
		sf::Sprite retryButton_;
		std::vector<Objective> objectives_;

		BoardPtrType board_;
		Match currentMatch_;
		GameController::GameDataPtrType data_;
		MatchPatternCollectionType matchPatterns_;

		int firstSelection_ = 0;
		int	secondSelection_ = 0;
		Status status_ = Status::WAITING;
	};

} // end namespace match3
#endif // MATCH3_GAMESTATE_HPP_