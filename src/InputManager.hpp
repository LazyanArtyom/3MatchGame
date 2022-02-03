#ifndef MATCH3_INPUTMANAGER_HPP_
#define MATCH3_INPUTMANAGER_HPP_

#include "SFML\Graphics.hpp"

namespace match3 {
	/**
	* \brief InputManager class manages events
	*/
	class InputManager
	{
	public:
		InputManager() = default;
		~InputManager() = default;

		sf::Vector2i getMousePosition(sf::RenderWindow& window);
		bool isSpriteClicked(sf::Sprite sprite, sf::Mouse::Button button, sf::RenderWindow& window);
	};

} // end namespace match3
#endif // MATCH3_INPUTMANAGER_HPP_