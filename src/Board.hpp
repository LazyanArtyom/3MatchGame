#ifndef MATCH3_BOARD_HPP_
#define MATCH3_BOARD_HPP_

#include "Figure.hpp"
#include <vector>

namespace match3 {

	/**
	* \brief Board class which keeps figures.
	* 
	* Board rows/cols size. (min 7, max 10)
	*/
	class Board
	{
	public:
		using FigureCollectionType = std::vector<Figure>;

		Board(GameController::GameDataPtrType data, int rows, int cols, int figureSize, int figurePadding, const sf::Vector2f& pos);
		~Board() = default;

		void init();

		int getRows() const;
		int getCols() const;
		int getFigureSize() const;

		void clear();
		void draw(sf::RenderWindow& window);

		Board::FigureCollectionType& getFigures();
		void addFigure(int col, int row, Figure::Color color, sf::Texture& texture, Figure::Status status);
		
	private:
		int rows_;
		int cols_;
		int figureSize_;
		int figurePadding_;

		sf::Vector2f pos_;
		FigureCollectionType figures_;
		GameController::GameDataPtrType data_;
	};

} // end namespace match3
#endif // MATCH3_BOARD_HPP_
