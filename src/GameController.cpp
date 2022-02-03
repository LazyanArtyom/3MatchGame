#include "GameController.hpp"
#include "MainMenuState.hpp"

#include <iostream>

namespace match3
{
	GameController::GameController(const std::string& resourceFilePath)
	{
		data_->configMgr.loadConfigFile(resourceFilePath, "config.json");

		// updates run per second.
		dt_ = 1.0f / data_->configMgr.getConfig<float>("FPS");

		int width = data_->configMgr.getConfig<int>("ScreenWidth");
		int height = data_->configMgr.getConfig<int>("ScreenHeight");
		std::string title = data_->configMgr.getConfig<std::string>("Title");

		data_->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
		data_->machine.addState(StateMachine::StatePtrType(new MainMenuState(data_)));
	}

	void GameController::run()
	{
		float newTime, frameTime;

		float currentTime = clock_.getElapsedTime().asSeconds();
		float accumulator = 0.0f;

		// starts the game according to the given fps in the config
		while (data_->window.isOpen())
		{
			data_->machine.processStateChanges();

			newTime = clock_.getElapsedTime().asSeconds();
			frameTime = newTime - currentTime;

			if (frameTime > 0.25f)
			{
				frameTime = 0.25f;
			}

			currentTime = newTime;
			accumulator += frameTime;

			while (accumulator >= dt_)
			{
				data_->machine.getActiveState()->handleInput();
				data_->machine.getActiveState()->update();

				accumulator -= dt_;
			}

			data_->machine.getActiveState()->draw();
		}
	}

} // end namespace match3