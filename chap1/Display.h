
#ifndef J_DISPLAY_H
#define J_DISPLAY_H

#include "MainWindow.h"
#include "TimelineWindow.h"
#include "WindowGroup.h"

#include "Jargon/SmartPointer.h"

namespace J{

	class Display {
		public:
			Display();

			void showSync();

			MainWindow & getMainWindow();
			TimelineWindow & getTimelineWindow();

		private:
			J::WindowGroup m_windowGroup;
			Jargon::SmartPointer<MainWindow> m_mainWindow;
			Jargon::SmartPointer<TimelineWindow> m_timelineWindow;
	};


}

#endif
