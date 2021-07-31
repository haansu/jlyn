#include "Program.h"

namespace jlyn {

	Program::Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable)
		: m_Title(_title), m_Resizeable(_resizeable) {
		m_VideoMode.width = _width;
		m_VideoMode.height = _height;
		m_PathIndex = 0;

		if (_resizeable)
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
		else
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close);

		m_Window->setFramerateLimit(60);

		m_View = m_Window->getDefaultView();
		InitObjects();

		// Move to it's own function + make able to read from any folder (+folder that the exe opens in)
		for (auto& files : std::filesystem::directory_iterator("Textures")) {
			if (IsSupported(files)) {
				m_FilePaths.push_back(files.path().string());
				CORE_TRACE("File path added: {0}", m_FilePaths[m_FilePaths.size() - 1]);
			}
		}

		CORE_INFO("{0} supported files have been found in the folder!", m_FilePaths.size());

		ImageRenderer(m_FilePaths[m_PathIndex]);

	}

	Program::~Program() {
		delete m_Window;
		delete m_Image;
	}

	void Program::InitObjects() {
		m_ButtonPrev.setSize(sf::Vector2f(100.0f, 100.0f));
		m_ButtonPrev.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_ButtonPrev.setFillColor(sf::Color(255, 0, 0, 180));

		m_ButtonNext.setSize(sf::Vector2f(100.0f, 100.0f));
		m_ButtonNext.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_ButtonNext.setFillColor(sf::Color(255, 0, 0, 180));
	}

	bool Program::IsSupported(std::filesystem::directory_entry _files) {
		std::string supFormats[8];

		// Supported formats
		supFormats[0] = ".png";
		supFormats[1] = ".jpg";
		supFormats[2] = ".bmp";
		supFormats[3] = ".gif";
		supFormats[4] = ".psd";
		supFormats[5] = ".hdr";
		supFormats[6] = ".pic";
		supFormats[7] = ".tga";

		std::string path = _files.path().string();

		// Checks if the path ends in any of the supported formats
		for (int i = 0; i < 8; i++)
			if (path.size() >= supFormats[i].size() && 0 == path.compare(path.size() - supFormats[i].size(), supFormats[i].size(), supFormats[i]))
				return true;

		return false;
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

						if (m_Event.key.code == sf::Keyboard::Left) {
							if (m_PathIndex > 0) {
								ImageRenderer(m_FilePaths[m_PathIndex - 1]);
								m_PathIndex--;
							}
						}

						if (m_Event.key.code == sf::Keyboard::Right) {
							if (m_PathIndex < m_FilePaths.size() - 1) {
								ImageRenderer(m_FilePaths[m_PathIndex + 1]);
								m_PathIndex++;
							}
						}
						break;
					}
					case sf::Event::Resized: {

						if (m_Window->getSize().x < 800)
							m_Window->setSize(sf::Vector2u(800, m_Event.size.height));

						if (m_Window->getSize().y < 600)
							m_Window->setSize(sf::Vector2u(m_Event.size.width, 600));
						
						m_View.setSize({
							static_cast<float>(m_Event.size.width),
							static_cast<float>(m_Event.size.height)
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

	// Needs to be initialised and changed when button click rather than every frame
	void Program::ImageRenderer(std::string _path) {
		CORE_TRACE("Loading immage from path: {0}", _path);

		
		delete m_Image;
		m_Image = new sf::Sprite;

		sf::Image image;
		if (!(image.loadFromFile(_path)))
			CORE_ERROR("Cannot load image from: {0}", _path);

		float scaleX = (static_cast<float>(m_Window->getSize().x) / static_cast<float>(image.getSize().x));
		float scaleY = (static_cast<float>(m_Window->getSize().y) / static_cast<float>(image.getSize().y));
		
		if (scaleX < scaleY)
			m_Image->setScale(scaleX, scaleX);
		else
			m_Image->setScale(scaleY, scaleY);

		m_Texture.loadFromImage(image);
		m_Image->setTexture(m_Texture);
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
		m_Window->draw(*m_Image);
		m_Window->draw(m_ButtonPrev);
		m_Window->draw(m_ButtonNext);
	}

}