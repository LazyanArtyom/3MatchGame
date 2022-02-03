#ifndef MATCH3_GAMECONTROLLER_HPP_
#define MATCH3_GAMECONTROLLER_HPP_

#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "InputManager.hpp"
#include "ConfigManager.hpp"

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

namespace match3 {

	/**
	* \brief GameController runs the game.
	*
	* Creates the main window and passes the GameData to the MainMenuState
	*/
	class GameController
	{
	public:
		struct GameData
		{
			StateMachine machine;
			InputManager inputMgr;
			AssetManager assetMgr;
			ConfigManager configMgr;
			sf::RenderWindow window;
			std::string resourceFilePath;
		};
		using GameDataPtrType = std::shared_ptr<GameData>;

	public:
		GameController(const std::string& resourceFilePath);

		void run();

	private:
		sf::Clock clock_;
		GameDataPtrType data_ = std::make_shared<GameData>();

		// updates run per second.
		float dt_ = 0.0f;
	};

} // end namespace match3
#endif // MATCH3_GAMECONTROLLER_HPP_