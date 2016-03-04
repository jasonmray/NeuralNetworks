
#include "Display.h"

namespace J{

	Display::Display() {
		m_mainWindow = new J::MainWindow();
		m_timelineWindow = new J::TimelineWindow();

		m_windowGroup.addWindow(m_mainWindow, true);
		m_windowGroup.addWindow(m_timelineWindow);
	}

	void Display::showSync() {
		m_windowGroup.runEventLoopSync();
	}

	MainWindow & Display::getMainWindow(){
		return *m_mainWindow;
	}

	TimelineWindow & Display::getTimelineWindow(){
		return *m_timelineWindow;
	}
}
