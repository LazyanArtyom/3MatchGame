#ifndef MATCH3_FIGURE_HPP_
#define MATCH3_FIGURE_HPP_

#include "State.hpp"
#include "GameController.hpp"
#include <SFML/Graphics.hpp>

namespace match3 {

	/**
	* \brief Represents a Figure in Board.
	*/
	class Figure
	{
	public:
		enum class Color : int
		{
			RED, ORANGE, GREEN, BLUE, PURPLE, RADIAL_BOMB, HORIZONTAL_BOMB, VERTICAL_BOMB, COUNT
		};

		enum class Status : int
		{
			NEW, NONE, SELECTED, MATCH, MOVING, DELETING, DELETED
		};

		Figure(int col, int row, int size, int marginLeft, int marginTop, int padding,
			Color color, sf::Texture& texture, Status status);

		Status update();
		void draw(sf::RenderWindow& window);

		void swapTargets(Figure& other);
		bool checkHit(const sf::Vector2f& pos) const;

		int getCol() const;
		int getRow() const;

		Color getColor() const;
		void setColor(Color color);

		Status getStatus() const;
		void setStatus(Status status);

		int getSize() const;
		int getPadding() const;
		int getMarginTop() const;
		int getMarginLeft() const;

		bool checkStatus(Status status) const;

	private:
		int col_ = 0;
		int row_ = 0;
		int alpha_ = 255;
		int padding_ = 0;
		int marginTop_ = 0;
		int marginLeft_ = 0;
		int figureSize_ = 0;

		sf::Vector2f pos_;
		sf::Vector2f size_;
		sf::Vector2f target_;
		sf::Sprite sprite_;

		Color color_;
		Status status_;
	};

	/**
	* \brief Represents a Objective.
	*
	* Game has objectives. Objectives are defined in the game config section.
	* After successful match, if the collected figures are in the list of objectives,
	* the objectives number should go down by the number of collected figures.
	* If all objectives are completed (counter of objective is 0), the game is considered complete.
	*/
	class Objective
	{
	public:
		Objective(int size, const sf::Vector2f& pos, Figure::Color color,
			sf::Texture& texture, const std::string& text, sf::Font& font);

		~Objective() = default;

		void draw(sf::RenderWindow& window);

		int getWidth() const;
		int getHeight() const;

		Figure::Color getColor() const;
		void setColor(Figure::Color color);

		int getCount() const;
		void setCount(int count);

	private:
		sf::Text text_;
		sf::Sprite sprite_;
		Figure::Color color_;
	};

} // end namespace match3
#endif // MATCH3_FIGURE_HPP_