#ifndef MATCH3_ASSETMANAGER_HPP_
#define MATCH3_ASSETMANAGER_HPP_

#include <map>
#include <string_view>
#include <SFML/Graphics.hpp>

namespace match3 {

	/**
	* \brief AssetManager class which loads and provides fonst and textures.
	*/
	class AssetManager
	{
		using FontCollectionType = std::map<std::string, sf::Font>;
		using TextureCollectionType = std::map<std::string, sf::Texture>;

	public:
		AssetManager() = default;
		~AssetManager() = default;

		sf::Texture& getTexture(const std::string& name);
		void loadTexture(const std::string& name, const std::string& fileName);

		sf::Font& getFont(const std::string& name);
		void loadFont(const std::string& name, const std::string& fileName);

	private:
		FontCollectionType fonts_;
		TextureCollectionType textures_;
	};

} // end namespace match3
#endif // MATCH3_ASSETMANAGER_HPP_