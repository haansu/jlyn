#include "Program.h"

namespace jlyn {

	Program::Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable, const char* _path, const char* _imagePath)
		: m_Title(_title), m_Resizeable(_resizeable) {
		m_VideoMode.width = _width;
		m_VideoMode.height = _height;
		m_Path = _path;
		m_BackgroundColor = sf::Color(25, 25, 25, 255);

		m_ZoomLevel = 1;
		m_ZoomSmooth = 1;

		m_Offset = sf::Vector2i(0, 0);

		int lastSlashIndex = m_Path.find_last_of('\\', m_Path.size() - 1);
		CORE_INFO("Last {0}", lastSlashIndex);
		m_Path = m_Path.substr(0, lastSlashIndex + 1);

		m_ImagePath = _imagePath;

		if (_imagePath != "[]none") {
			lastSlashIndex = m_ImagePath.find_last_of('\\', m_ImagePath.size() - 1);
			m_ImageDirPath = m_ImagePath.substr(0, lastSlashIndex + 1);

			CORE_TRACE("Image path: {0} \nImage directory path: {1}", m_ImagePath, m_ImageDirPath);
		}
		else {
			CORE_FATAL("Image path: {0}", _imagePath);
			m_ImagePath = "Textures/1.jpg";
			m_ImageDirPath = "Textures";
		}

		m_PathIndex = 0;

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
	}

	// Runs the application
	void Program::Run() {
		while (m_Window->isOpen()) {
			CheckEvents();
			Update();

			m_Window->clear(m_BackgroundColor);
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
					break;
				}

				// Adds offset when typing and records the offset value necesary for every zoom out
				// Also changes immages if no zoom
				if (m_Event.key.code == sf::Keyboard::Left) {
					if (m_ZoomLevel <= 1)
						PrevImage();
					else {
						m_Offset.x += 25;
						if (m_ZoomLevel != 1)
							m_OffsetRecenter.x = m_Offset.x / ((m_ZoomLevel - 1) * 10);
					}
				}

				if (m_Event.key.code == sf::Keyboard::Right) {
					if(m_ZoomLevel <= 1)
						NextImage();
					else {
						m_Offset.x -= 25;
						if (m_ZoomLevel != 1)
						m_OffsetRecenter.x = m_Offset.x / ((m_ZoomLevel - 1) * 10);
					}
				}

				if (m_Event.key.code == sf::Keyboard::Up) {
					if (m_ZoomLevel > 1)
						m_Offset.y += 25;
					m_OffsetRecenter.y = m_Offset.y / ((m_ZoomLevel - 1) * 10);
				}

				if (m_Event.key.code == sf::Keyboard::Down) {
					if (m_ZoomLevel > 1)
						m_Offset.y -= 25;
					m_OffsetRecenter.y = m_Offset.y / ((m_ZoomLevel - 1) * 10);
				}
				ImageUpdate();
				break;
			}

			case sf::Event::MouseButtonPressed: {
				if (m_ButtonPrev.Hovered(m_Window)) {
					PrevImage();
					break;
				}

				if (m_ButtonNext.Hovered(m_Window)) {
					NextImage();
					break;
				}

				break;
			}

			case sf::Event::MouseWheelMoved: {
				if (m_ZoomLevel > 0.3 && m_Event.mouseWheel.delta < 0) {
					m_ZoomLevel -= 0.1;

					if (m_OffsetRecenter.x != 0 && m_ZoomLevel > 1)
						m_Offset.x -= m_OffsetRecenter.x;
					if (m_OffsetRecenter.y != 0 && m_ZoomLevel > 1)
						m_Offset.y -= m_OffsetRecenter.y;

					// Offsets reset when there's no zoom
					if (m_ZoomLevel == 1) {
						m_Offset.y = 0;
						m_Offset.x = 0;
						m_OffsetRecenter.x = 0;
						m_OffsetRecenter.y = 0;
					}

				}
				if (m_ZoomLevel < 3 && m_Event.mouseWheel.delta > 0) {
					m_ZoomLevel += 0.1;
				}

				m_ZoomDelta = m_ZoomLevel - m_ZoomSmooth;

				CORE_TRACE("Zoom Level: {0}", m_ZoomLevel);
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

				m_View.setCenter((float)m_Window->getSize().x / 2, (float)m_Window->getSize().y / 2);
				m_Window->setView(m_View);

				ButtonUpdate(m_Window);
				ImageUpdate();
			}
			}
		}
	}

	// Initialisez objects to be drawn on screen
	void Program::InitObjects() {
		// Left arrow button
		m_ButtonPrev.Init("Prev-Button", sf::Vector2f(70.0f, 100.0f), sf::Vector2f(0.0f, 0.0f), sf::Color(255, 255, 255, 180));
		m_ButtonPrev.LoadTexture((m_Path + "\\sprites\\arrow_left.png"));

		// Right arrow button
		m_ButtonNext.Init("Next-Button", sf::Vector2f(70.0f, 100.0f), sf::Vector2f(0.0f, 0.0f), sf::Color(255, 255, 255, 180));
		m_ButtonNext.LoadTexture((m_Path + "\\sprites\\arrow_right.png"));

		// Left and right pads
		m_LeftPad.Init("Left-Pad", sf::Vector2f(100.0f, 100.0f), sf::Vector2f(0.0f, 0.0f), m_BackgroundColor);
		m_RightPad.Init("Right-Pad", sf::Vector2f(100.0f, 100.0f), sf::Vector2f(0.0f, 0.0f), m_BackgroundColor);
		m_TopPad.Init("Top-Pad", sf::Vector2f(100.0f, 100.0f), sf::Vector2f(0.0f, 0.0f), sf::Color(15, 15, 15, 255));
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

		m_ZoomSmooth = 1;
		m_ZoomLevel  = 1;

		m_Offset = sf::Vector2i(0, 0);

		delete m_Image;
		m_Image = new sf::Sprite;

		sf::Image image;

		if (!(image.loadFromFile(_path)))
			CORE_ERROR("Cannot load image from: {0}", _path);

		float scaleX = (static_cast<float>(m_Window->getSize().x) / static_cast<float>(image.getSize().x)) * 0.85f * m_ZoomSmooth;
		float scaleY = (static_cast<float>(m_Window->getSize().y) / static_cast<float>(image.getSize().y)) * 0.85f * m_ZoomSmooth;

		if (scaleX < scaleY)
			m_Image->setScale(scaleX, scaleX);
		else
			m_Image->setScale(scaleY, scaleY);

		m_Texture.loadFromImage(image);
		m_Texture.setSmooth(true);
		m_Image->setTexture(m_Texture);

		// Offsetting the image so it's always centered
		int imgOffsetX = (m_Window->getSize().x - (m_Image->getTexture()->getSize().x * m_Image->getScale().x)) / 2;
		int imgOffsetY = (m_Window->getSize().y + 50 - (m_Image->getTexture()->getSize().y * m_Image->getScale().x)) / 2;

		m_Image->setPosition(m_Window->mapPixelToCoords(sf::Vector2{ imgOffsetX, imgOffsetY }));
	}

	void Program::ImageUpdate() {

		float scaleX = (static_cast<float>(m_Window->getSize().x) / static_cast<float>(m_Image->getTexture()->getSize().x)) * 0.85f * m_ZoomSmooth;
		float scaleY = (static_cast<float>(m_Window->getSize().y) / static_cast<float>(m_Image->getTexture()->getSize().y)) * 0.85f * m_ZoomSmooth;

		if (scaleX < scaleY)
			m_Image->setScale(scaleX, scaleX);
		else
			m_Image->setScale(scaleY, scaleY);

		int imgOffsetX = ((m_Window->getSize().x - (m_Image->getTexture()->getSize().x * m_Image->getScale().x)) / 2) + m_Offset.x;
		int imgOffsetY = ((m_Window->getSize().y + 50 - (m_Image->getTexture()->getSize().y * m_Image->getScale().x)) / 2) + m_Offset.y;

		m_Image->setPosition(sf::Vector2f(imgOffsetX, imgOffsetY));
	}

	void Program::ButtonUpdate(sf::RenderWindow*& _window) {
		sf::Vector2i buttonPrevPos;
		buttonPrevPos.x = 0;
		buttonPrevPos.y = ((float)_window->getSize().y + 50) / 2 - m_ButtonPrev.GetSize().y / 2;

		sf::Vector2i buttonNextPos;
		buttonNextPos.x = (float)_window->getSize().x - m_ButtonNext.GetSize().x;
		buttonNextPos.y = ((float)_window->getSize().y + 50) / 2 - m_ButtonNext.GetSize().y / 2;

		m_ButtonPrev.SetPosition(sf::Vector2f(buttonPrevPos));
		m_ButtonNext.SetPosition(sf::Vector2f(buttonNextPos));

		CORE_INFO("{0}, {1}", m_ButtonPrev.GetPosition().x, m_ButtonPrev.GetPosition().y);

		// Pads update. To be moved to own function
		sf::Vector2f padSize;
		padSize.x = 30;
		padSize.y = _window->getSize().y;

		m_LeftPad.SetPositionRel(_window, sf::Vector2i(0.0f, 0.0f));
		m_RightPad.SetPositionRel(_window, sf::Vector2i(_window->getSize().x - padSize.x, 0.0f));

		m_TopPad.SetPositionRel(_window, sf::Vector2i(0.0f, 0.0f));
		m_TopPad.SetSize(sf::Vector2f((float)_window->getSize().x, 50));

		m_LeftPad.SetSize(padSize);
		m_RightPad.SetSize(padSize);
	}

	// Changes image to the previous in the list
	void Program::PrevImage() {
		if (m_PathIndex > 0) {
			ImageRenderer(m_FilePaths[m_PathIndex - 1]);
			m_PathIndex--;
		}
	}

	// Changes image to the next in the list
	void Program::NextImage() {
		if (m_PathIndex < m_FilePaths.size() - 1) {
			ImageRenderer(m_FilePaths[m_PathIndex + 1]);
			m_PathIndex++;
		}
	}

	// Fades buttons on hover
	// Feels a little hacky, will find a better solution
	void Program::FadeInOut(Button& _button) {
		if (_button.Hovered(m_Window)) {
			if (_button.GetOpacity() < 255) {
				if (_button.GetOpacity() > 245) {
					_button.SetOpacity(255);
					return;
				}

				if (_button.GetOpacity() < 100)
					_button.SetOpacity(_button.GetOpacity() + 20);
				else
					_button.SetOpacity(_button.GetOpacity() + 10);
			}
		}
		else if (_button.GetOpacity() > 0) {
			if (m_ZoomLevel > 1) {
				if (_button.GetOpacity() <= 10) {
					_button.SetOpacity(0);
					return;
				}

				_button.SetOpacity(_button.GetOpacity() - 10);
			} else if (_button.GetOpacity() >= 100)
				_button.SetOpacity(_button.GetOpacity() - 10);
		}
		
		if (!_button.Hovered(m_Window) && _button.GetOpacity() < 100 && m_ZoomLevel <= 1) {
			_button.SetOpacity(_button.GetOpacity() + 10);
		}
	}

	// Updates proprierties on every frame
	void Program::Update() {
		FadeInOut(m_ButtonPrev);
		FadeInOut(m_ButtonNext);

		// Smooth zooming
		if (m_ZoomLevel != m_ZoomSmooth) {
			m_ZoomSmooth += (m_ZoomDelta / 10);
			if (m_ZoomSmooth < m_ZoomLevel + 0.01 && m_ZoomSmooth > m_ZoomLevel - 0.01)
				m_ZoomSmooth = m_ZoomLevel;
			ImageUpdate();
		}
	}

	// Renders the screen every frame | Drawn in order
	void Program::Render() {
		m_Window->draw(*m_Image);
		
		m_LeftPad.Draw(m_Window);
		m_RightPad.Draw(m_Window);

		m_TopPad.Draw(m_Window);

		m_ButtonNext.Draw(m_Window);
		m_ButtonPrev.Draw(m_Window);
	}

}
