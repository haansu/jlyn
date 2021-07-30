#pragma once

#include <SFML.h>

namespace jlyn {

	class Program {
	public:
		Program();			
		Program(unsigned int _width, unsigned int _height, std::string _title, bool _resizeable);
		~Program();

		void Run();

		void Update();

		void Render();
	private:
		std::string m_Title;
		bool m_Resizeable;

		sf::RenderWindow* m_Window;
		sf::VideoMode m_VideoMode;
		sf::Event m_Event;
	};

}