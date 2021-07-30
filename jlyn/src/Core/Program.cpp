#include "Program.h"

namespace jlyn {

	Program::Program()
		: m_Title("JLyn"), m_Resizeable(false) {
		m_VideoMode.width   = 1280;
		m_VideoMode.height	= 720;

		m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close);	

		m_View = m_Window->getDefaultView();
		InitObjects();
	}

	Program::Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable)
		: m_Title(_title), m_Resizeable(_resizeable) {
		m_VideoMode.width	= _width;
		m_VideoMode.height	= _height;

		if (_resizeable)
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
		else
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close);
		
		m_View = m_Window->getDefaultView();
		InitObjects();
	}

	Program::~Program() {
		delete m_Window;
	}

	void Program::InitObjects() {
		m_ButtonPrev.setSize(sf::Vector2f(100.0f, 100.0f));
		m_ButtonPrev.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_ButtonPrev.setFillColor(sf::Color::Red);

		m_ButtonNext.setSize(sf::Vector2f(100.0f, 100.0f));
		m_ButtonNext.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_ButtonNext.setFillColor(sf::Color::Red);
	}

	void Program::Run() {
		while (m_Window->isOpen()) {
			while (m_Window->pollEvent(m_Event)) {
				switch (m_Event.type) {
					case sf::Event::Closed: {
						m_Window->close();
						CORE_INFO("Window closed!");
						break;
					}
					case sf::Event::KeyPressed: {
						CORE_TRACE("Key pressed: {0}", m_Event.text.unicode);
						if (m_Event.key.code == sf::Keyboard::Escape) {
							m_Window->close();
							CORE_INFO("Window closed!");
						}
						break;
					}
					case sf::Event::Resized: {
						m_View.setSize({
							static_cast<float>(m_Event.size.width),
							static_cast<float>(m_Event.size.height)
						});

						if (m_Window->getSize().x < 800)
							m_Window->setSize(sf::Vector2u(800, m_Event.size.height));

							m_View.setSize({
							800.0f,
							static_cast<float>(m_Event.size.height)
						});

						if (m_Window->getSize().y < 600)
							m_Window->setSize(sf::Vector2u(m_Event.size.width, 600));

							m_View.setSize({
							static_cast<float>(m_Event.size.width),
							600.0f
						});

						m_Window->setView(m_View);
					}
				}
			}

			Update();

			m_Window->clear(sf::Color(55, 55, 55, 255));

			Render();

			m_Window->display();
		}
	}

	void Program::Update() {
		sf::Vector2f buttonPrevPos;
		buttonPrevPos.x = 0;
		buttonPrevPos.y = m_Window->getSize().y / 2 - m_ButtonPrev.getSize().y / 2;
		
		sf::Vector2f buttonNextPos;
		buttonNextPos.x = m_Window->getSize().x - m_ButtonNext.getSize().x;
		buttonNextPos.y = m_Window->getSize().y / 2 - m_ButtonNext.getSize().y / 2;


		m_ButtonPrev.setPosition(m_Window->mapPixelToCoords(sf::Vector2i{ buttonPrevPos }));
		m_ButtonNext.setPosition(m_Window->mapPixelToCoords(sf::Vector2i{ buttonNextPos }));
	}
	
	void Program::Render() {
		m_Window->draw(m_ButtonPrev);
		m_Window->draw(m_ButtonNext);
	}

}