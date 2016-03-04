
#ifndef J_WINDOW_H
#define J_WINDOW_H

#include "Jargon/SmartPointer.h"
#include "Jargon/ScopedPointer.h"
#include "SFML/Graphics.hpp"


namespace J{

	class Window : public Jargon::ReferenceCountableBase {
		public:
			Window(sf::VideoMode & videoMode, const char * title);
			Window(sf::VideoMode & videoMode, const char * title, int positionX, int positionY);

			virtual ~Window();

			virtual void draw(sf::RenderWindow & window) = 0;

			void clear();
			void close();

			sf::RenderWindow & getRenderWindow();

		private:
			Jargon::ScopedPointer<sf::RenderWindow> m_window;
	};
}

#endif
