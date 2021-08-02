#pragma once

#include <SFML.h>
#include "Entity.h"

namespace jlyn {

	class Button : public Entity {
	public:
		Button();
		Button(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color, std::string _path);
		~Button();

		void Init(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color);
		void Draw(sf::RenderWindow*& _window);

		void SetSize(sf::Vector2f& _size); 
		void SetPosition(sf::Vector2f& _position);
		void SetPositionRel(sf::RenderWindow*& _window, sf::Vector2i _position);
		void SetColor(sf::Color& _color);
		void LoadTexture(std::string _path);

		sf::Vector2f GetSize();
	private:
		sf::RectangleShape m_Button;
		sf::Texture* m_Texture = new sf::Texture;
	};

}
