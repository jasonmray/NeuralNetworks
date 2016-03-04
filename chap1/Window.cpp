
#include "Window.h"
#include <SFML/OpenGL.hpp>

namespace J{

	Window::Window(sf::VideoMode & videoMode, const char * title){
		sf::ContextSettings contextSettings;
		contextSettings.antialiasingLevel = 4;

		m_window = new sf::RenderWindow(videoMode, title, 7, contextSettings);
		m_window->setActive(false);
		m_window->setVerticalSyncEnabled(true);
	}

	Window::Window(sf::VideoMode & videoMode, const char * title, int positionX, int positionY){
		sf::ContextSettings contextSettings;
		contextSettings.antialiasingLevel = 4;

		m_window = new sf::RenderWindow(videoMode, title, 7, contextSettings);
		m_window->setActive(false);
		m_window->setVerticalSyncEnabled(true);

		sf::Vector2i position(positionX, positionY);
		m_window->setPosition(position);
	}
	

	Window::~Window(){
	}

	void Window::clear() {
		m_window->clear(sf::Color::White);
	}

	void Window::close() {
		m_window->close();
	}

	sf::RenderWindow & Window::getRenderWindow() {
		return *m_window;
	}

}
