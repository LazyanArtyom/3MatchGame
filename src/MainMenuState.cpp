#include "MainMenuState.hpp"
#include "GameState.hpp"

#include <sstream>
#include <iostream>

namespace match3
{
	MainMenuState::MainMenuState(GameController::GameDataPtrType data) : data_(data) {}

	void MainMenuState::init()
	{
		int screenWidth = data_->configMgr.getConfig<int>("ScreenWidth");
		int screenHeight = data_->configMgr.getConfig<int>("ScreenHeight");

		// background
		data_->assetMgr.loadTexture("MainMenuBackground", data_->configMgr.getConfig<std::string>("MainMenuBackground"));
		background_.setTexture(data_->assetMgr.getTexture("MainMenuBackground"));

		// title
		data_->assetMgr.loadTexture("Game Title", data_->configMgr.getConfig<std::string>("MainMenuTitle"));
		title_.setTexture(data_->assetMgr.getTexture("Game Title"));
		title_.setPosition(float((screenWidth / 2) - (title_.getGlobalBounds().width / 2)), float(title_.getGlobalBounds().height * 0.1));
		
		// play button
		data_->assetMgr.loadTexture("Play Button", data_->configMgr.getConfig<std::string>("MainMenuPlayButton"));
		playButton_.setTexture(data_->assetMgr.getTexture("Play Button"));
		playButton_.setPosition((screenWidth / 2) - (playButton_.getGlobalBounds().width / 2), (screenHeight / 2) - (playButton_.getGlobalBounds().height / 2));
	}

	void MainMenuState::handleInput()
	{
		sf::Event event;

		while (data_->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				data_->window.close();
			}

			if (data_->inputMgr.isSpriteClicked(playButton_, sf::Mouse::Left, data_->window))
			{
				// switch to Game State
				data_->machine.addState(StateMachine::StatePtrType(new GameState(data_)), true);
			}
		}
	}

	void MainMenuState::update()
	{
		// nothing to update
	}

	void MainMenuState::draw()
	{
		data_->window.clear();

		data_->window.draw(background_);
		data_->window.draw(playButton_);
		data_->window.draw(title_);

		data_->window.display();
	}

} // end namespace match3