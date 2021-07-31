#pragma once

#include <SFML.h>
#include <JLynCore.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string_view>

namespace jlyn {
	
	// Takes care of running the application and the window
	class Program {
	public:
		Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable);
		~Program();

		void Run();
		void CheckEvents();
		
		void InitObjects();
		void ScanDirectory(std::string _directory);
		bool IsSupported(std::filesystem::directory_entry _files);

		void ImageRenderer(std::string _path);

		void Update();
		void Render();
	private:
		std::string m_Title;
		bool m_Resizeable;

		std::vector<std::string> m_FilePaths;
		unsigned int m_PathIndex;

		sf::RenderWindow* m_Window;
		sf::VideoMode m_VideoMode;
		sf::Event m_Event;
		sf::View m_View;

		sf::RectangleShape m_ButtonPrev;
		sf::RectangleShape m_ButtonNext;

		sf::Sprite* m_Image;
		sf::Texture m_Texture;

		sf::Texture* m_LeftButtonTexture;
		sf::Texture* m_RightButtonTexture;
	};

}