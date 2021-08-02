#include "Button.h"

namespace jlyn {

	Button::Button() 
		: Entity(){
		SetSize(sf::Vector2f(100.0f, 100.0f));
		SetPosition(sf::Vector2f(0.0f, 0.0f));
		SetColor(sf::Color(255, 255, 255, 255));
	}

	Button::Button(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color, std::string _path)
		: Entity(_title) {
		m_Button.setSize(_size);
		m_Button.setPosition(_position);
		m_Button.setFillColor(_color);
		LoadTexture(_path);
	}

	Button::~Button() {
		delete m_Texture;
	}

	void Button::Init(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color) {
		m_Title = _title;
		m_Button.setSize(_size);
		m_Button.setPosition(_position);
		m_Button.setFillColor(_color);
	}

	void Button::Draw(sf::RenderWindow*& _window) {
		_window->draw(m_Button);
	}

	void Button::SetSize(sf::Vector2f& _size) {
		m_Button.setSize(_size);
	}

	void Button::SetPosition(sf::Vector2f& _position) {
		m_Button.setPosition(_position);
	}

	void Button::SetPositionRel(sf::RenderWindow*& _window, sf::Vector2i _position) {
		m_Button.setPosition(_window->mapPixelToCoords(sf::Vector2i{ _position }));
	}

	void Button::SetColor(sf::Color& _color) {
		m_Button.setFillColor(_color);
	}

	sf::Vector2f Button::GetSize() {
		return m_Button.getSize();
	}

	void Button::LoadTexture(std::string _path) {
		sf::Image buttonImg;
		CORE_INFO("Image loading loading for {0}...", m_Title);
		if (!(buttonImg.loadFromFile((_path).c_str())))
			CORE_ERROR("Cannot load left_arrow from: {0}", _path);
		m_Texture->loadFromImage(buttonImg);
		m_Button.setTexture(m_Texture);
	}

}