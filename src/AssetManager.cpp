#include "AssetManager.hpp"
#include "ErrorManager.hpp"
#include <SFML/Graphics.hpp>

namespace match3 {

	sf::Texture& AssetManager::getTexture(const std::string& name)
	{
		try
		{
			return textures_.at(name);
		}
		catch (const std::exception& e)
		{
			ErrorManager::print("AssetManager", e.what());
		}
	}

	void AssetManager::loadTexture(const std::string& name, const std::string& fileName)
	{
		sf::Texture tex;

		if (tex.loadFromFile(fileName))
		{
			textures_[name] = tex;
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

	sf::Font& AssetManager::getFont(const std::string& name)
	{
		try
		{
			return fonts_.at(name);
		}
		catch (const std::exception& e)
		{
			ErrorManager::print("AssetManager", e.what());
		}
	}

	void AssetManager::loadFont(const std::string& name, const std::string& fileName)
	{
		sf::Font font;

		if (font.loadFromFile(fileName))
		{
			fonts_[name] = font;
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}

} // end namespace match3