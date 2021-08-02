#pragma once

#include <SFML.h>
#include "Entity.h"

namespace jlyn {

	class Pad : public Entity {
	public:
		Pad();
		Pad(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color);
		~Pad() = default;

		void Init(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color);
		void Draw(sf::RenderWindow*& _window);

		void SetSize(sf::Vector2f& _size);
		void SetPosition(sf::Vector2f& _position);
		void SetPositionRel(sf::RenderWindow*& _window, sf::Vector2i _position);
		void SetColor(sf::Color& _color);

		sf::Vector2f GetSize();
	private:
		sf::RectangleShape m_Pad;
	};

}
