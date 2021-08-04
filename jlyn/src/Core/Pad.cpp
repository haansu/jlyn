#include "Pad.h"

namespace jlyn {

	Pad::Pad()
		: Entity() {
		CORE_INFO("Pad {0} has been created!", m_Title);
		SetSize(sf::Vector2f(100.0f, 100.0f));
		SetPosition(sf::Vector2f(0.0f, 0.0f));
		SetColor(sf::Color(255, 255, 255, 255));
	}

	Pad::Pad(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color)
		: Entity(_title) {
		CORE_INFO("Pad {0} has been created!", m_Title);
		m_Pad.setSize(_size);
		m_Pad.setPosition(_position);
		m_Pad.setFillColor(_color);
	}

	void Pad::Init(std::string _title, sf::Vector2f _size, sf::Vector2f _position, sf::Color _color) {
		m_Pad.setSize(_size);
		CORE_INFO("Initialising {0}!", m_Title);
		m_Pad.setPosition(_position);
		m_Pad.setFillColor(_color);
	}

	void Pad::Draw(sf::RenderWindow*& _window) {
		_window->draw(m_Pad);
	}

	void Pad::SetSize(sf::Vector2f& _size) {
		m_Pad.setSize(_size);
	}

	void Pad::SetPosition(sf::Vector2f& _position) {
		m_Pad.setPosition(_position);
	}

	// Sets position relative to the window pixelmap
	void Pad::SetPositionRel(sf::RenderWindow*& _window, sf::Vector2i _position) {
		m_Pad.setPosition(_window->mapPixelToCoords(sf::Vector2i{ _position }));
	}

	void Pad::SetColor(sf::Color& _color) {
		m_Pad.setFillColor(_color);
	}

	sf::Vector2f Pad::GetSize() {
		return m_Pad.getSize();
	}
}
