#include "GameOverState.hpp"
#include "GameState.hpp"

namespace match3
{
	GameOverState::GameOverState(GameController::GameDataPtrType data, bool isWon) : isWon_(isWon), data_(data) {}

	void GameOverState::init()
	{
		int screenWidth = data_->configMgr.getConfig<int>("ScreenWidth");
		int screenHeight = data_->configMgr.getConfig<int>("ScreenHeight");
		
		// background
		data_->assetMgr.loadTexture("GameOverBackground", data_->configMgr.getConfig<std::string>("GameOverBackground"));
		background_.setTexture(data_->assetMgr.getTexture("GameOverBackground"));

		// retry button
		retryButton_.setTexture(data_->assetMgr.getTexture("RetryButton"));
		retryButton_.setPosition((screenWidth / 2) - (retryButton_.getGlobalBounds().width / 2),
			(screenHeight / 2) - (retryButton_.getGlobalBounds().height / 2));

		// title
		title_.setFont(data_->assetMgr.getFont("Font"));
		title_.setCharacterSize(64);
		title_.setFillColor(sf::Color::White);

		if (isWon_)
		{
			title_.setString("You Won");
		}
		else
		{
			title_.setString("You Loose");
		}
		
		// center of the screen
		title_.setPosition((screenWidth / 2) - (title_.getGlobalBounds().width / 2),
			(screenHeight / 2) - (title_.getGlobalBounds().height + retryButton_.getGlobalBounds().height));
	}

	void GameOverState::handleInput()
	{
		sf::Event event;

		while (data_->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				data_->window.close();
			}

			if (data_->inputMgr.isSpriteClicked(retryButton_, sf::Mouse::Left, data_->window))
			{
				// switch To Game State
				data_->machine.addState(StateMachine::StatePtrType(new GameState(data_)), true);
			}
		}
	}

	void GameOverState::update()
	{
		// nothing to update
	}

	void GameOverState::draw()
	{
		data_->window.clear();

		data_->window.draw(background_);
		data_->window.draw(title_);
		data_->window.draw(retryButton_);

		data_->window.display();
	}

} // end namespace match3