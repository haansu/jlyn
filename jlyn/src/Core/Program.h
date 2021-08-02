#pragma once

#include <SFML.h>
#include <JLynCore.h>
#include <Core/Button.h>

namespace jlyn {
	
	// Takes care of everything application at the moment. Needs DESPAGETTIFICATION
	class Program {
	public:
		Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable, const char* _path, const char* _imagePath);
		~Program();

		void Run();
		void CheckEvents();
		
		void InitObjects();
		void ScanDirectory(std::string _directory, unsigned int& _index);
		bool IsSupported(std::filesystem::directory_entry _files);

		void ImageRenderer(std::string _path);
		void ButtonUpdate(sf::RenderWindow*& _window);

		void Update();
		void Render();
	private:
		std::string m_Title;
		bool m_Resizeable;

		std::string m_Path;
		std::string m_ImagePath;
		std::string m_ImageDirPath;

		std::vector<std::string> m_FilePaths;
		unsigned int m_PathIndex;

		sf::RenderWindow* m_Window;
		sf::VideoMode m_VideoMode;
		sf::Event m_Event;
		sf::View m_View;

		Button m_ButtonPrev;
		Button m_ButtonNext;

		sf::Sprite* m_Image;
		sf::Texture m_Texture;

		sf::Texture* m_LeftButtonTexture;
		sf::Texture* m_RightButtonTexture;
	};

}
