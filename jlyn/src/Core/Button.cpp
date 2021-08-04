#include "Button.h"

namespace jlyn {

	Button::Button() 
		: Entity(){
		CORE_INFO("Button {0} has been created!", m_Title);
		SetSize(sf::Vector2f(100.0f, 100.0f));
		SetPosition(sf::Vector2f(0.0f, 0.0f));
		SetColor(sf::Color(255, 255, 255, 255));
	}

	Button::Button(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color, std::string _path)
		: Entity(_title) {
		CORE_INFO("Button {0} has been created!", m_Title);
		m_Button.setSize(_size);
		m_Button.setPosition(_position);
		m_Button.setFillColor(_color);
		LoadTexture(_path);
	}

	Button::~Button() {
		delete m_Texture;
	}

	// Initialisez button
	void Button::Init(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color) {
		m_Title = _title;
		CORE_INFO("Initialising {0}!", m_Title);
		m_Button.setSize(_size);
		m_Button.setPosition(_position);
		m_Button.setFillColor(_color);
	}

	// Draws the button on a renderwindow
	void Button::Draw(sf::RenderWindow*& _window) {
		_window->draw(m_Button);
	}

	void Button::SetSize(sf::Vector2f& _size) {
		m_Button.setSize(_size);
	}

	void Button::SetPosition(sf::Vector2f& _position) {
		m_Button.setPosition(_position);
	}

	// Sets position relative to the window pixelmap
	void Button::SetPositionRel(sf::RenderWindow*& _window, sf::Vector2i& _position) {
		m_Button.setPosition(_window->mapPixelToCoords(sf::Vector2i{ _position }));
	}

	void Button::SetOpacity(sf::Uint8 _opacity) {
		sf::Color modifiedAlpha = m_Button.getFillColor();
		modifiedAlpha.a = _opacity;
		SetColor(modifiedAlpha);
	}

	void Button::SetColor(sf::Color& _color) {
		m_Button.setFillColor(_color);
	}

	// Checks if the object is hovered
	bool Button::Hovered(sf::RenderWindow*& _window) {
		if (sf::Mouse::getPosition(*_window).x > m_Button.getPosition().x && sf::Mouse::getPosition(*_window).x < m_Button.getPosition().x + m_Button.getSize().x &&
			sf::Mouse::getPosition(*_window).y > m_Button.getPosition().y && sf::Mouse::getPosition(*_window).y < m_Button.getPosition().y + m_Button.getSize().y) {
			CORE_TRACE("Hovering over {0}", m_Title);
			return true;
		}
		return false;
	}

	float Button::GetOpacity() {
		return m_Button.getFillColor().a;
	}

	sf::Color Button::GetColor() {
		return m_Button.getFillColor();
	}
	
	sf::Vector2f Button::GetSize() {
		return m_Button.getSize();
	}

	sf::Vector2f Button::GetPosition() {
		return m_Button.getPosition();
	}

	// Loads the texture into the texure component of the buttton
	void Button::LoadTexture(std::string _path) {
		sf::Image buttonImg;
		CORE_INFO("Image loading loading for {0}...", m_Title);
		if (!(buttonImg.loadFromFile((_path).c_str())))
			CORE_ERROR("Cannot load left_arrow from: {0}", _path);
		m_Texture->loadFromImage(buttonImg);
		m_Button.setTexture(m_Texture);
	}

}
