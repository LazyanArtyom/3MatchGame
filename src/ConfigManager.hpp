#ifndef MATCH3_CONFIGMANAGER_HPP_
#define MATCH3_CONFIGMANAGER_HPP_

#include "json.hpp"
#include "ErrorManager.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

namespace match3 {
	/**
	* \brief ConfigManager class which loads and provides configs.
	*
	* if the config file json does not exist,
	* then we generate new one with default params in resource directory path.
	*/
	class ConfigManager
	{
	public:
		ConfigManager() = default;
		~ConfigManager() = default;

		template<typename T>
		const T getConfig(const std::string& config)
		{
			if (json_.contains(config))
			{
				return json_[config].get<T>();
			}
			else
			{
				std::string err = config + ": config does not exist.";
				ErrorManager::print("ConfigManager", err);
			}
		}

		void loadConfigFile(const std::string& resourceFilePath, const std::string& fileName)
		{
			if (!std::filesystem::exists(resourceFilePath + fileName))
			{
				generateDefaultConfigFile(resourceFilePath, fileName);
			}
			else
			{
				try
				{
					std::ifstream ifs(resourceFilePath + fileName);

					// allow exceptions and ignore comments in json file
					json_ = nlohmann::json::parse(ifs, nullptr, true, true);
				}
				catch (const nlohmann::detail::exception& e)
				{
					ErrorManager::print("ConfigManager", e.what());
				}
			}
		}

		bool contains(const std::string& config)
		{
			return json_.contains(config);
		}

		void generateDefaultConfigFile(const std::string& resourceFilePath, const std::string& fileName)
		{
			// if the config file does not exist, then we generate new one with default params
			std::ofstream ofs(resourceFilePath + fileName);

			json_["Title"] = "3Match Game";
			json_["ScreenWidth"] = 744;
			json_["ScreenHeight"] = 1080;
			json_["FPS"] = 60.0;

			json_["MainMenuTitle"] = resourceFilePath + "game_title.png";
			json_["MainMenuBackground"] = resourceFilePath + "main_menu_background.jpg";
			json_["MainMenuPlayButton"] = resourceFilePath + "play_button.png";

			json_["Font"] = resourceFilePath + "sansation.ttf";
			json_["Tile1"] = resourceFilePath + "tile_1.png";
			json_["Tile2"] = resourceFilePath + "tile_2.png";
			json_["Figures"] = resourceFilePath + "figures.png";
			json_["RetryButton"] = resourceFilePath + "retry_button.png";
			json_["GameBackground"] = resourceFilePath + "game_background.png";

			json_["GameOverBackground"] = resourceFilePath + "game_background.png";

			json_["BoardRows"] = 8;
			json_["BoardCols"] = 8;
			json_["FigureSize"] = 64;
			json_["FigurePadding"] = 4;
			json_["MovesCount"] = "15";
			json_["FiguresColorsCount"] = 5;

			json_["ObjectivesCount"] = 3;
			json_["red"] = "3";
			json_["green"] = "5";
			json_["blue"] = "8";

			// prettyfied
			ofs << std::setw(4) << json_ << std::endl;
		}

	private:
		nlohmann::ordered_json json_;
	};

} // end namespace match3
#endif // MATCH3_CONFIGMANAGER_HPP_