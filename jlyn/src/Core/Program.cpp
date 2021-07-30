#include "Program.h"

namespace jlyn {
	Program::Program()
		: m_Title("JLyn"), m_Resizeable(false) {
		m_VideoMode.width   = 1280;
		m_VideoMode.height	= 720;
		
		m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close);
	}

	Program::Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable)
		: m_Title(_title), m_Resizeable(_resizeable) {
		m_VideoMode.width	= _width;
		m_VideoMode.height	= _height;

		if (_resizeable)
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
		else
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close);
	}

	Program::~Program() {
		delete m_Window;
	}



	void Program::Run() {
		while (m_Window->isOpen()) {
			while (m_Window->pollEvent(m_Event)) {
				switch (m_Event.type) {
				case sf::Event::Closed:
					m_Window->close();
					break;
				case sf::Event::KeyPressed:
					if (m_Event.key.code == sf::Keyboard::Escape)
						m_Window->close();
					break;
				}
			}

			m_Window->clear(sf::Color(55, 55, 55, 255));

			m_Window->display();
		}
	}

}