#include "InputManager.hpp"

namespace match3 {

	sf::Vector2i InputManager::getMousePosition(sf::RenderWindow& window)
	{
		return sf::Mouse::getPosition(window);
	}

	bool InputManager::isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window)
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::IntRect playButtonRect(sprite.getPosition().x, sprite.getPosition().y,
				sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

			if (playButtonRect.contains(sf::Mouse::getPosition(window)))
			{
				return true;
			}
		}

		return false;
	}

} // end namespace match3