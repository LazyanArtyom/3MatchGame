#include "Figure.hpp"

namespace match3 {

	Figure::Figure(int col, int row, int size, int marginLeft, int marginTop, int padding, Color color, sf::Texture& texture, Status status)
		: col_(col), row_(row), figureSize_(size), marginLeft_(marginLeft),
		marginTop_(marginTop), padding_(padding), color_(color), status_(status), sprite_(texture)
	{
		size_ = sf::Vector2f(float(figureSize_), float(figureSize_));
		pos_ = sf::Vector2f(float(marginLeft_ + (size_.x + padding_) * col), float(marginTop_ + (size_.y + padding_) * row));
		target_ = pos_;

		sprite_.setTextureRect(sf::IntRect(static_cast<int>(color) * figureSize_, 0, figureSize_, figureSize_));
		sprite_.setColor(sf::Color(255, 255, 255, alpha_));
		sprite_.setPosition(pos_);
	}

	void Figure::draw(sf::RenderWindow& window)
	{
		if (!checkStatus(Figure::Status::NEW))
		{
			sprite_.setPosition(pos_);
			sprite_.setTextureRect(sf::IntRect(static_cast<int>(color_) * figureSize_,
				(checkStatus(Status::SELECTED)) ? figureSize_ : 0, figureSize_, figureSize_));

			sprite_.setColor(sf::Color(255, 255, 255, alpha_));
			window.draw(sprite_);
		}
	}

	bool Figure::checkHit(const sf::Vector2f& pos) const
	{
		if (checkStatus(Status::DELETED))
		{
			return false;
		}
		return sf::Rect<float>(pos_, size_).contains(pos);
	}

	int Figure::getCol() const
	{
		return col_;
	}

	int Figure::getRow() const
	{
		return row_;
	}

	Figure::Color Figure::getColor() const
	{
		return color_;
	}

	void Figure::setColor(Color color)
	{
		color_ = color;
	}

	void Figure::swapTargets(Figure& other)
	{
		std::swap(col_, other.col_);
		std::swap(row_, other.row_);

		target_ = sf::Vector2f(float(marginLeft_ + (size_.x + padding_) * col_),
			float(marginTop_ + (size_.y + padding_) * row_));

		other.target_ = sf::Vector2f(float(marginLeft_ + (size_.x + padding_) * other.col_),
			float(marginTop_ + (size_.y + padding_) * other.row_));

		setStatus(Status::MOVING);
		other.setStatus(Status::MOVING);
	}

	Figure::Status Figure::update()
	{
		switch (status_)
		{
		case Status::MATCH:
		{
			setStatus(Status::DELETING);
			break;
		}
		case Status::DELETING:
		{
			alpha_ -= 10;
			if (alpha_ <= 50)
			{
				setStatus(Status::DELETED);
			}
			break;
		}
		case Status::MOVING:
		case Status::DELETED:
		{
			auto move = [=](float& pos, float target)
			{
				if (pos == target)
				{
					return false;
				}

				if (pos < target)
				{
					pos += std::min(5.0f, target - pos);
				}
				else
				{
					pos -= std::min(5.0f, pos - target);
				}

				return true;
			};

			if (!move(pos_.x, target_.x) && !move(pos_.y, target_.y) && checkStatus(Status::MOVING))
			{
				setStatus(Status::NONE);
			}
			break;
		}
		default:
			break;
		}

		return status_;
	}

	Figure::Status Figure::getStatus() const
	{
		return status_;
	}

	void Figure::setStatus(Status status)
	{
		if (status_ != Status::DELETED)
		{
			status_ = status;
		}
	}

	bool Figure::checkStatus(Status status) const
	{
		return status_ == status;
	}

	int Figure::getSize() const
	{
		return figureSize_;
	}

	int Figure::getPadding() const
	{
		return padding_;
	}

	int Figure::getMarginTop() const
	{
		return marginTop_;
	}

	int Figure::getMarginLeft() const
	{
		return marginLeft_;
	}

	Objective::Objective(int size, const sf::Vector2f& pos, Figure::Color color, 
		sf::Texture& texture, const std::string& text, sf::Font& font)
	{
		color_ = color;
		
		int padding = 20;
		sprite_.setPosition(pos.x + padding, pos.y);
		sprite_.setTexture(texture);
		sprite_.setTextureRect(sf::IntRect(static_cast<int>(color) * size, 0, size, size));

		text_.setFont(font);
		text_.setCharacterSize(46);
		text_.setString(text);
		text_.setFillColor(sf::Color::White);
		text_.setPosition(pos.x + sprite_.getGlobalBounds().width + padding, 0);
	}

	void Objective::draw(sf::RenderWindow& window)
	{
		window.draw(sprite_);
		window.draw(text_);
	}

	int Objective::getWidth() const
	{
		return int(sprite_.getGlobalBounds().width + text_.getGlobalBounds().width);
	}

	int Objective::getHeight() const
	{
		return int(sprite_.getGlobalBounds().height + text_.getGlobalBounds().height);
	}

	Figure::Color Objective::getColor() const
	{
		return color_;
	}

	void Objective::setColor(Figure::Color color)
	{
		color_ = color;
	}

	int Objective::getCount() const
	{
		return std::stoi(text_.getString().toAnsiString());
	}

	void Objective::setCount(int count)
	{
		text_.setString(std::to_string(count));
	}

} // end namespace match3