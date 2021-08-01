#include "Program.h"

namespace jlyn {

	Program::Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable, const char* _path, const char* _imagePath)
		: m_Title(_title), m_Resizeable(_resizeable) {
		m_VideoMode.width	= _width;
		m_VideoMode.height	= _height;
		m_Path				= _path;
		int lastSlashIndex = m_Path.find_last_of('\\', m_Path.size() - 1);
		CORE_INFO("Last {0}", lastSlashIndex);
		m_Path				= m_Path.substr(0, lastSlashIndex + 1);
		
		m_ImagePath = _imagePath; 

		if (_imagePath != "[]none") {
			lastSlashIndex = m_ImagePath.find_last_of('\\', m_ImagePath.size() - 1);
			m_ImageDirPath = m_ImagePath.substr(0, lastSlashIndex + 1);

			CORE_TRACE("Image path: {0} \nImage directory path: {1}", m_ImagePath, m_ImageDirPath);
		} else {
			CORE_FATAL("Image path: {0}", _imagePath);
			m_ImagePath = "Textures/1.jpg";
			m_ImageDirPath = "Textures";
		}


		m_PathIndex	= 0;

		m_LeftButtonTexture	= new sf::Texture;
		m_RightButtonTexture = new sf::Texture;

		if (_resizeable)
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
		else
			m_Window = new sf::RenderWindow(m_VideoMode, m_Title, sf::Style::Titlebar | sf::Style::Close);

		m_Window->setFramerateLimit(30);

		m_View = m_Window->getDefaultView();
		
		m_Image = new sf::Sprite;
		InitObjects();

		ScanDirectory(m_ImageDirPath, m_PathIndex);
		ButtonUpdate(m_Window);
		ImageRenderer(m_FilePaths[m_PathIndex]);
	}

	Program::~Program() {
		delete m_Window;
		delete m_Image;
		delete m_LeftButtonTexture;
		delete m_RightButtonTexture;
	}

	// Runs the application
	void Program::Run() {
		while (m_Window->isOpen()) {
			CheckEvents();
			Update();

			m_Window->clear(sf::Color(25, 25, 25, 255));
			Render();
			m_Window->display();
		}
	}

	// Checks events and acts on them
	void Program::CheckEvents() {
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


					CORE_INFO("Window: w: {0} h: {1}", m_Window->getSize().x, m_Window->getSize().y);

					if (m_Window->getSize().x < 800)
						m_Window->setSize(sf::Vector2u(800, m_Event.size.height));

					if (m_Window->getSize().y < 600)
						m_Window->setSize(sf::Vector2u(m_Event.size.width, 600));

					m_View.setSize({
						static_cast<float>(m_Event.size.width),
						static_cast<float>(m_Event.size.height)
					});
					


					m_Window->setView(m_View);

					ButtonUpdate(m_Window);
					ImageRenderer(m_FilePaths[m_PathIndex]);
				}
			}
		}
	}

	// Initialisez objects to be drawn on screen
	void Program::InitObjects() {
		// Left arrow button
		m_ButtonPrev.setSize(sf::Vector2f(100.0f, 100.0f));
		m_ButtonPrev.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_ButtonPrev.setFillColor(sf::Color(255, 255, 255, 180));

		sf::Image buttonImg;

		CORE_INFO("Image loading arrow_left...");
		if (!(buttonImg.loadFromFile((m_Path + "\\sprites\\arrow_left.png").c_str())))
			CORE_ERROR("Cannot load left_arrow from: {0}", "\\sprites\\arrow_left.png");

		m_LeftButtonTexture->loadFromImage(buttonImg);
		m_ButtonPrev.setTexture(m_LeftButtonTexture);

		// Right arrow button
		m_ButtonNext.setSize(sf::Vector2f(100.0f, 100.0f));
		m_ButtonNext.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_ButtonNext.setFillColor(sf::Color(255, 255, 255, 180));

		CORE_INFO("Image loading arrow_right...");
		if (!(buttonImg.loadFromFile(m_Path + "\\sprites\\arrow_right.png")))
			CORE_ERROR("Cannot load left_arrow from: {0}", (m_Path + "\\sprites\\arrow_right.png").c_str());

		m_RightButtonTexture->loadFromImage(buttonImg);
		m_ButtonNext.setTexture(m_RightButtonTexture);
	}

	// Scans the directory for all supported image files
	void Program::ScanDirectory(std::string _directory, unsigned int& _index) {
		for (auto& files : std::filesystem::directory_iterator(_directory)) {
			if (IsSupported(files)) {
				m_FilePaths.push_back(files.path().string());

				if (m_ImagePath == m_FilePaths[m_FilePaths.size() - 1])
					_index = m_FilePaths.size() - 1;
				
				CORE_TRACE("File path added: {0}", m_FilePaths[m_FilePaths.size() - 1]);
			}
		}

		CORE_INFO("{0} supported files have been found in the folder!", m_FilePaths.size());
	}

	// Checks if the image file is of a supported format
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

	// Configures the sprite to be rendered on screen in Render()
	void Program::ImageRenderer(std::string _path) {
		CORE_TRACE("Loading immage from path: {0}", _path);

		delete m_Image;

		m_Image = new sf::Sprite;

		sf::Image image;

		if (!(image.loadFromFile(_path)))
			CORE_ERROR("Cannot load image from: {0}", _path);

		float scaleX = (static_cast<float>(m_Window->getSize().x) / static_cast<float>(image.getSize().x)) * 0.87f;
		float scaleY = (static_cast<float>(m_Window->getSize().y) / static_cast<float>(image.getSize().y)) * 0.87f;
		
		if (scaleX < scaleY)
			m_Image->setScale(scaleX, scaleX);
		else
			m_Image->setScale(scaleY, scaleY);

		m_Texture.loadFromImage(image);
		m_Texture.setSmooth(true);
		m_Image->setTexture(m_Texture);

		// Move later to a getOffset function
		int imgOffsetX = (m_Window->getSize().x - (m_Image->getTexture()->getSize().x * m_Image->getScale().x)) / 2;
		int imgOffsetY = (m_Window->getSize().y - (m_Image->getTexture()->getSize().y * m_Image->getScale().x)) / 2;

		m_Image->setPosition(m_Window->mapPixelToCoords(sf::Vector2{ imgOffsetX, imgOffsetY }));
	}

	void Program::ButtonUpdate(sf::RenderWindow*& _window) {
		sf::Vector2f buttonPrevPos;
		buttonPrevPos.x = 0;
		buttonPrevPos.y = _window->getSize().y / 2 - m_ButtonPrev.getSize().y / 2;

		sf::Vector2f buttonNextPos;
		buttonNextPos.x = _window->getSize().x - m_ButtonNext.getSize().x;
		buttonNextPos.y = _window->getSize().y / 2 - m_ButtonNext.getSize().y / 2;

		m_ButtonPrev.setPosition(m_Window->mapPixelToCoords(sf::Vector2i{ buttonPrevPos }));
		m_ButtonNext.setPosition(m_Window->mapPixelToCoords(sf::Vector2i{ buttonNextPos }));
	}

	// Updates proprierties on every frame
	void Program::Update() {
		// Will do calculations for zoom bar
	}

	// Renders the screen every frame | Drawn in order
	void Program::Render() {
		m_Window->draw(*m_Image);
		m_Window->draw(m_ButtonPrev);
		m_Window->draw(m_ButtonNext);
	}

}
