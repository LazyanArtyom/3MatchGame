#include "Board.hpp"
#include "ErrorManager.hpp"

namespace match3 {

	Board::Board(GameController::GameDataPtrType data, int rows, int cols, int figureSize, int figurePadding, const sf::Vector2f& pos)
		: data_(data), rows_(rows), cols_(cols), figureSize_(figureSize), figurePadding_(figurePadding), pos_(pos)
	{
		init();
	}

	void Board::init()
	{
		// constraint checking 
		if (rows_ < 7 || rows_ > 10 || cols_ < 7 || cols_ > 10)
		{
			ErrorManager::print("Board", "Board rows/cols size. (min 7, max 10).");
		}

		data_->assetMgr.loadTexture("Tile1", data_->configMgr.getConfig<std::string>("Tile1"));
		data_->assetMgr.loadTexture("Tile2", data_->configMgr.getConfig<std::string>("Tile2"));
		data_->assetMgr.loadTexture("Figures", data_->configMgr.getConfig<std::string>("Figures"));
	}

	int Board::getRows() const
	{
		return rows_;
	}

	int Board::getCols() const
	{
		return cols_;
	}

	int Board::getFigureSize() const
	{
		return figureSize_;
	}

	void Board::clear()
	{
		figures_.clear();
	}

	void Board::draw(sf::RenderWindow& window)
	{
		// draw background board
		for (int row = 0; row < rows_; ++row)
		{
			for (int col = 0; col < cols_; ++col)
			{
				sf::Sprite sprite;
				if ((col + row) % 2 == 0)
				{
					sprite.setTexture(data_->assetMgr.getTexture("Tile1"));
				}
				else
				{
					sprite.setTexture(data_->assetMgr.getTexture("Tile2"));
				}

				sprite.setTextureRect(sf::IntRect(figureSize_ + 4, figureSize_ + 3, figureSize_ + 4, figureSize_ + 4));
				sprite.setPosition(float(pos_.x + sprite.getGlobalBounds().width * col), float(pos_.y + sprite.getGlobalBounds().height * row));
				data_->window.draw(sprite);
			}
		}

		// draw figures
		for (auto& figure : figures_)
		{
			figure.draw(window);
		}
	}

	Board::FigureCollectionType& Board::getFigures()
	{
		return figures_;
	}

	void Board::addFigure(int col, int row, Figure::Color color, sf::Texture& texture, Figure::Status status)
	{
		// constraint checking 
		if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
		{
			ErrorManager::print("Board", "Out of board range.");
		}

		figures_.emplace_back(col, row, figureSize_, pos_.x, pos_.y, figurePadding_, color, texture, status);
	}

} // end namespace match3
