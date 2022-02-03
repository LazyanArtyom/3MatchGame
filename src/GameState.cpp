#include "GameState.hpp"
#include "MainMenuState.hpp"
#include "GameOverState.hpp"

namespace match3 {

	GameState::GameState(GameController::GameDataPtrType data) : data_(data) {}

	void GameState::init()
	{
		int screenWidth = data_->configMgr.getConfig<int>("ScreenWidth");
		int screenHeight = data_->configMgr.getConfig<int>("ScreenHeight");

		// init board
		int rows = data_->configMgr.getConfig<int>("BoardRows");
		int cols = data_->configMgr.getConfig<int>("BoardCols");
		int figureSize = data_->configMgr.getConfig<int>("FigureSize");
		int figurePadding = data_->configMgr.getConfig<int>("FigurePadding");
		board_ = std::make_unique<Board>(data_, rows, cols, figureSize, figurePadding,
			sf::Vector2f(float((screenWidth / 2) - (figureSize * cols) / 2), float((screenHeight / 2) - (figureSize * rows) / 2)));

		// background
		data_->assetMgr.loadTexture("GameBackground", data_->configMgr.getConfig<std::string>("GameBackground"));
		background_.setTexture(data_->assetMgr.getTexture("GameBackground"));

		// retry button
		data_->assetMgr.loadTexture("RetryButton", data_->configMgr.getConfig<std::string>("RetryButton"));
		retryButton_.setTexture(data_->assetMgr.getTexture("RetryButton"));
		retryButton_.setPosition(screenWidth - retryButton_.getGlobalBounds().width, 0);

		// moves count 
		data_->assetMgr.loadFont("Font", data_->configMgr.getConfig<std::string>("Font"));
		movesCount_.setFont(data_->assetMgr.getFont("Font"));
		movesCount_.setCharacterSize(64);
		movesCount_.setPosition((screenWidth / 2) - (movesCount_.getGlobalBounds().width / 2), 0);
		movesCount_.setFillColor(sf::Color::White);

		colorsCount_ = data_->configMgr.getConfig<int>("FiguresColorsCount");

		initMatchPatterns();

		srand(static_cast<int>(time(nullptr)));
		reset();
	}

	void GameState::handleInput()
	{
		sf::Event event;

		while (data_->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				data_->window.close();
			}
			else if (sf::Event::MouseButtonPressed == event.type)
			{
				click(data_->window.mapPixelToCoords(sf::Mouse::getPosition(data_->window)));
			}

			if (data_->inputMgr.isSpriteClicked(retryButton_, sf::Mouse::Left, data_->window))
			{
				reset();
			}
		}
	}

	void GameState::update()
	{
		Board::FigureCollectionType& figures = board_->getFigures();

		if (checkStatus(Status::WAITING))
		{
			for (auto& figure : figures)
			{
				if (figure.checkStatus(Figure::Status::NEW))
				{
					figure.setStatus(Figure::Status::NONE);
				}
			}

			match();

			if (currentMatch_.count > 0)
			{
				checkForBombs();
				checkObjectives();
				checkEndGame();

				setStatus(Status::MOVING);
			}
			else if (arrange())
			{
				setStatus(Status::MOVING);
			}
			else if (!checkStatus(Status::MOVING) && !checkStatus(Status::SWAPPING))
			{
				isFirstArrange_ = false;
			}
		}

		if (checkStatus(Status::MOVING) || checkStatus(Status::SWAPPING))
		{
			bool moving = false;
			for (auto& figure : figures)
			{
				Figure::Status status = figure.update();
				if (status == Figure::Status::MOVING || status == Figure::Status::DELETING)
				{
					moving = true;
				}
			}

			if (!moving)
			{
				if (checkStatus(Status::SWAPPING))
				{
					match();

					// return back swapped figures
					if (currentMatch_.count == 0)
					{
						auto& firstFigure = figures[firstSelection_];
						auto& secondFigure = figures[secondSelection_];
						std::iter_swap(&firstFigure, &secondFigure);
						secondFigure.swapTargets(firstFigure);
					}
					else
					{
						checkForBombs();
						checkObjectives();
						decrementMove();
						checkEndGame();
					}

					setStatus(Status::MOVING);
				}
				else
				{
					setStatus(Status::WAITING);
				}
			}
		}
	}

	void GameState::reset()
	{
		resetFigures();
		resetObjectives();
		resetMovesCount();

		isFirstArrange_ = true;
		setStatus(Status::WAITING);
	}

	Figure::Color GameState::getRandomColor() const
	{
		return  static_cast<Figure::Color>(rand() % colorsCount_);
	}

	void GameState::resetFigures()
	{
		board_->clear();

		for (int row = 0; row < board_->getRows(); ++row)
		{
			for (int col = 0; col < board_->getCols(); ++col)
			{
				board_->addFigure(col, row, getRandomColor(), data_->assetMgr.getTexture("Figures"), Figure::Status::NONE);
			}
		}
	}

	void GameState::resetMovesCount()
	{
		movesCount_.setString(data_->configMgr.getConfig<std::string>("MovesCount"));
	}

	void GameState::resetObjectives()
	{
		objectives_.clear();

		bool isColorAdded[static_cast<int>(Figure::Color::COUNT)] = { false };
		int objectivesCount = data_->configMgr.getConfig<int>("ObjectivesCount");

		int size = board_->getFigureSize();

		sf::Vector2f pos;
		Figure::Color color;
		sf::Font font = data_->assetMgr.getFont("Font");

		for (int i = 0; i < objectivesCount; ++i)
		{
			if (i == 0)
			{
				pos.x = 0.0f;
				pos.y = 0.0f;
			}
			else
			{
				pos.x = float(objectives_[0].getWidth() * i);
			}
			
			// check which objectives are added in config file 
			if (data_->configMgr.contains("red") && !isColorAdded[static_cast<int>(Figure::Color::RED)])
			{
				color = Figure::Color::RED;
				isColorAdded[static_cast<int>(Figure::Color::RED)] = true;
				objectives_.emplace_back(
					size,
					pos,
					color,
					data_->assetMgr.getTexture("Figures"),
					data_->configMgr.getConfig<std::string>("red"),
					data_->assetMgr.getFont("Font"));
			}
			else if (data_->configMgr.contains("orange") && !isColorAdded[static_cast<int>(Figure::Color::ORANGE)])
			{
				color = Figure::Color::ORANGE;
				isColorAdded[static_cast<int>(Figure::Color::ORANGE)] = true;
				objectives_.emplace_back(
					size,
					pos,
					color,
					data_->assetMgr.getTexture("Figures"),
					data_->configMgr.getConfig<std::string>("orange"),
					data_->assetMgr.getFont("Font"));
			}
			else if (data_->configMgr.contains("green") && !isColorAdded[static_cast<int>(Figure::Color::GREEN)])
			{
				color = Figure::Color::GREEN;
				isColorAdded[static_cast<int>(Figure::Color::GREEN)] = true;
				objectives_.emplace_back(
					size,
					pos,
					color,
					data_->assetMgr.getTexture("Figures"),
					data_->configMgr.getConfig<std::string>("green"),
					data_->assetMgr.getFont("Font"));
			}
			else if (data_->configMgr.contains("blue") && !isColorAdded[static_cast<int>(Figure::Color::BLUE)])
			{
				color = Figure::Color::BLUE;
				isColorAdded[static_cast<int>(Figure::Color::BLUE)] = true;
				objectives_.emplace_back(
					size,
					pos,
					color,
					data_->assetMgr.getTexture("Figures"),
					data_->configMgr.getConfig<std::string>("blue"),
					data_->assetMgr.getFont("Font"));
			}
			else if (data_->configMgr.contains("purple") && !isColorAdded[static_cast<int>(Figure::Color::PURPLE)])
			{
				color = Figure::Color::PURPLE;
				isColorAdded[static_cast<int>(Figure::Color::PURPLE)] = true;
				objectives_.emplace_back(
					size,
					pos,
					color,
					data_->assetMgr.getTexture("Figures"),
					data_->configMgr.getConfig<std::string>("purple"),
					data_->assetMgr.getFont("Font"));
			}
		}
	}

	void  GameState::setStatus(Status status)
	{
		status_ = status;
	}

	bool GameState::checkStatus(Status status) const
	{
		return status_ == status;
	}

	void GameState::click(const sf::Vector2f& pos)
	{
		if (!checkStatus(Status::WAITING) && !checkStatus(Status::SELECTED))
		{
			return;
		}

		Board::FigureCollectionType& figures = board_->getFigures();

		for (auto& figure : figures)
		{
			if (figure.checkHit(pos))
			{
				Figure::Color color = figure.getColor();

				// if bomb clicked then explode
				if (color == Figure::Color::RADIAL_BOMB ||
					color == Figure::Color::VERTICAL_BOMB ||
					color == Figure::Color::HORIZONTAL_BOMB)
				{
					return explode(&figure);
				}
			
				// if first is selected then do swap
				if (checkStatus(Status::SELECTED))
				{
					// swap figures
					secondSelection_ = figure.getRow() * board_->getCols() + figure.getCol();
					auto& other = figures[firstSelection_];

					if ((abs(figure.getCol() - other.getCol()) + abs(figure.getRow() - other.getRow())) != 1)
					{
						break;
					}

					std::iter_swap(&figure, &other);
					other.swapTargets(figure);
					setStatus(Status::SWAPPING);
				}
				// select figure
				else
				{
					firstSelection_ = figure.getRow() * board_->getCols() + figure.getCol();
					figure.setStatus(Figure::Status::SELECTED);
					setStatus(Status::SELECTED);
				}
				break;
			}
		}
	}

	void GameState::decrementMove()
	{
		if (isFirstArrange_)
		{
			return;
		}

		int count = getMovesCount();
		--count;
		movesCount_.setString(std::to_string(count));
	}

	int GameState::getMovesCount() const
	{
		return std::stoi(movesCount_.getString().toAnsiString());
	}

	void GameState::initMatchPatterns()
	{
		// horizontal match
		matchPatterns_[MatchType::HORIZONTAL] = [this](int row, int col, auto& figure) {

			int match = 0;
			if (col > 0 && col < (board_->getCols() - 1))
			{
				match = matchColor({ &*figure, &*(figure + 1), &*(figure - 1) });
			}
			return match;
		};

		// vertical match
		matchPatterns_[MatchType::VERTICAL] = [this](int row, int col, auto& figure) {

			int match = 0;
			if (row > 0 && row < board_->getRows() - 1)
			{
				match = matchColor({ &*figure, &*(figure + board_->getCols()), &*(figure - board_->getCols()) });
			}

			return match;
		};

		// radial match
		matchPatterns_[MatchType::RADIAL] = [this](int row, int col, auto& figure) {

			int match = 0;
			if (row < board_->getRows() - 1 && col < board_->getCols() - 1)
			{
				match = matchColor({ &*figure, &*(figure + board_->getCols()), &*(figure + 1), &*(figure + board_->getCols() + 1) });
			}

			return match;
		};
	}

	void GameState::match()
	{
		Board::FigureCollectionType& figures = board_->getFigures();
		currentMatch_ = { 0, MatchType::NONE, Figure::Color::RED };

		auto figure = figures.begin();

		for (int row = 0; row < board_->getRows(); ++row)
		{
			for (int col = 0; col < board_->getCols(); ++col, ++figure)
			{
				for (const auto& [patternType, pattern] : matchPatterns_)
				{
					int matchCount = pattern(row, col, figure);
					if (matchCount > 0)
					{
						currentMatch_.count += matchCount;
						currentMatch_.type = patternType;
						currentMatch_.color = figure->getColor();
					}
				}
			}
		}
	}
	
	// check if all figures has the same color
	int GameState::matchColor(std::vector<Figure*> figures)
	{
		int match = 0;
		Figure::Color color = figures[0]->getColor();

		for (const auto& figure : figures)
		{
			if (!figure->checkStatus(Figure::Status::NONE)
				&& !figure->checkStatus(Figure::Status::MATCH)
				|| figure->getColor() != color)
			{
				return 0;
			}
		}

		for (auto& figure : figures)
		{
			if (!figure->checkStatus(Figure::Status::MATCH))
			{
				++match;
				figure->setStatus(Figure::Status::MATCH);
			}
		}

		return match;
	}

	bool GameState::arrange()
	{
		bool arranging = false;
		Board::FigureCollectionType& figures = board_->getFigures();

		for (auto figure = figures.rbegin(); figure != figures.rend(); ++figure)
		{
			if (!figure->checkStatus(Figure::Status::DELETED))
			{
				continue;
			}

			if (figure->getRow() == 0)
			{
				break;
			}

			arranging = true;
			for (int row = 1; row <= figure->getRow(); ++row)
			{
				if (!(figure + (static_cast<int64_t>(board_->getCols()) * row))->checkStatus(Figure::Status::DELETED))
				{
					std::iter_swap(figure, (figure + (static_cast<int64_t>(board_->getCols()) * row)));
					figure->swapTargets(*(figure + ((int64_t)board_->getCols() * row)));
					break;
				}
			}
		}

		replaceDeleted();
		return arranging;
	}

	void GameState::replaceDeleted()
	{
		Board::FigureCollectionType& figures = board_->getFigures();

		for (auto figure = figures.begin(); figure != figures.end(); ++figure)
		{
			if (figure->checkStatus(Figure::Status::DELETED))
			{
				*figure = Figure(figure->getCol(), figure->getRow(), figure->getSize(), figure->getMarginLeft(),
					figure->getMarginTop(), figure->getPadding(), getRandomColor(), 
					data_->assetMgr.getTexture("Figures"), Figure::Status::NEW);
			}
		}
	}

	void GameState::explode(Figure* figure)
	{
		Figure::Color color = figure->getColor();
		Board::FigureCollectionType& figures = board_->getFigures();
		
		// recursive explosion if there is another bomb 
		switch (color)
		{
		case Figure::Color::RADIAL_BOMB:
		{
			for (int row = figure->getRow() - 1; row < figure->getRow() + 2; ++row)
			{
				for (int col = figure->getCol() - 1; col < figure->getCol() + 2; ++col)
				{
					if (col >= 0 && col < board_->getCols() && row >= 0 && row < board_->getRows())
					{
						int idx = row * board_->getCols() + col;
						if (figures[idx].getStatus() == Figure::Status::MATCH)
						{
							continue;
						}

						if (figures[idx].getColor() == Figure::Color::RADIAL_BOMB ||
							figures[idx].getColor() == Figure::Color::VERTICAL_BOMB ||
							figures[idx].getColor() == Figure::Color::HORIZONTAL_BOMB)
						{
							figures[idx].setStatus(Figure::Status::MATCH);
							explode(&figures[idx]);
						}
						figures[idx].setStatus(Figure::Status::MATCH);
					}
				}
			}
			break;
		}
		case Figure::Color::HORIZONTAL_BOMB:
		{
			for (int row = figure->getRow(), col = 0; col < board_->getCols(); ++col)
			{
				if (col >= 0 && col < board_->getCols() && row >= 0 && row < board_->getRows())
				{
					int idx = row * board_->getCols() + col;
					if (figures[idx].getStatus() == Figure::Status::MATCH)
					{
						continue;
					}

					if (figures[idx].getColor() == Figure::Color::RADIAL_BOMB ||
						figures[idx].getColor() == Figure::Color::VERTICAL_BOMB)
					{
						figures[idx].setStatus(Figure::Status::MATCH);
						explode(&figures[idx]);
					}
					figures[idx].setStatus(Figure::Status::MATCH);
				}
			}
			break;
		}
		case Figure::Color::VERTICAL_BOMB:
		{
			for (int row = 0, col = figure->getCol(); row < board_->getRows(); ++row)
			{
				if (col >= 0 && col < board_->getCols() && row >= 0 && row < board_->getRows())
				{
					int idx = row * board_->getCols() + col;
					if (figures[idx].getStatus() == Figure::Status::MATCH)
					{
						continue;
					}

					if (figures[idx].getColor() == Figure::Color::RADIAL_BOMB ||
						figures[idx].getColor() == Figure::Color::HORIZONTAL_BOMB)
					{
						figures[idx].setStatus(Figure::Status::MATCH);
						explode(&figures[idx]);
					}

					figures[idx].setStatus(Figure::Status::MATCH);
				}
			}
			break;
		}
		default:
			break;
		}

		decrementMove();
		setStatus(Status::MOVING);
	}

	void GameState::spawnBomb(Figure::Color color)
	{
		// add in a random place
		Board::FigureCollectionType& figures = board_->getFigures();

		int idx = (rand() % board_->getRows()) * board_->getCols() + (rand() % board_->getCols());
		figures[idx].setColor(color);
	}

	void GameState::checkEndGame()
	{
		if (checkStatus(Status::LOSING))
		{
			data_->machine.addState(StateMachine::StatePtrType(new GameOverState(data_, false)), true);
		}

		if (checkStatus(Status::WINNING))
		{
			data_->machine.addState(StateMachine::StatePtrType(new GameOverState(data_, true)), true);
		}
	}

	void GameState::checkForBombs()
	{
		if (isFirstArrange_)
		{
			return;
		}

		if (currentMatch_.type == MatchType::HORIZONTAL && currentMatch_.count >= 4)
		{
			spawnBomb(Figure::Color::HORIZONTAL_BOMB);
		}
		else if (currentMatch_.type == MatchType::VERTICAL && currentMatch_.count >= 4)
		{
			spawnBomb(Figure::Color::VERTICAL_BOMB);
		}
		else if (currentMatch_.type == MatchType::RADIAL && currentMatch_.count >= 4)
		{
			spawnBomb(Figure::Color::RADIAL_BOMB);
		}
	}

	void GameState::checkObjectives()
	{
		int count = 0;
		bool isWon = true;

		if (isFirstArrange_)
		{
			return;
		}

		for (auto& objective : objectives_)
		{
			if (objective.getColor() == currentMatch_.color && currentMatch_.count > 0)
			{
				count = (objective.getCount() - currentMatch_.count);

				objective.setCount(count > 0 ? count : 0);
			}

			if (objective.getCount() != 0)
			{
				isWon = false;
			}
		}

		if (isWon)
		{
			setStatus(Status::WINNING);
		}
	}

	void GameState::draw()
	{
		data_->window.clear();
		data_->window.draw(background_);
		data_->window.draw(movesCount_);
		data_->window.draw(retryButton_);

		board_->draw(data_->window);

		// draw objectives
		for (auto& objective : objectives_)
		{
			objective.draw(data_->window);
		}

		data_->window.display();
	}

} // end namespace match3