
#include "WindowGroup.h"

namespace J{

	WindowGroup::WindowGroup(){
	}

	WindowGroup::~WindowGroup(){
	}

	void WindowGroup::addWindow(const Jargon::SmartPointer<Window> & window, bool setAsMain) {
		m_windows.push_back(window);
		if( setAsMain ){
			m_mainWindow = window;
		}
	}

	void WindowGroup::closeAllWindows(){
		for (auto & window : m_windows) {
			window->close();
		}
	}

	void WindowGroup::runEventLoopSync() {
		sf::Event event;

		bool windowUpdated = true;

		while(windowUpdated){
			windowUpdated = false;

			for (auto & window : m_windows) {
				sf::RenderWindow & renderWindow(window->getRenderWindow());

				if (!renderWindow.isOpen()) {
					continue;
				}

				windowUpdated = true;

				while (renderWindow.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						renderWindow.close();
						if( window == m_mainWindow ){
							closeAllWindows();
							return;
						}
					}
				}
				window->clear();
				window->draw(renderWindow);
				renderWindow.display();
			}
		}
	}

}
