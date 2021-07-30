#pragma once

#include <SFML.h>
#include <JLynCore.h>

namespace jlyn {
	
	// Takes care of running the application and the window
	class Program {
	public:
		Program();			
		Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable);
		~Program();

		void Run();

		void InitObjects();

		void Update();
		void Render();
	private:
		std::string m_Title;
		bool m_Resizeable;

		sf::RenderWindow* m_Window;
		sf::VideoMode m_VideoMode;
		sf::Event m_Event;
		sf::View m_View;

		sf::RectangleShape m_ButtonPrev;
		sf::RectangleShape m_ButtonNext;
	};

}