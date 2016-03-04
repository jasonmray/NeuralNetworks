
#ifndef J_WINDOWGROUP_H
#define J_WINDOWGROUP_H

#include "Window.h"

#include "Jargon/SmartPointer.h"
#include "Jargon/Macros.h"

#include <vector>


namespace J{

	class WindowGroup{
		public:
			WindowGroup();
			~WindowGroup();

			void addWindow(const Jargon::SmartPointer<Window> & window, bool setAsMain = false);
			void runEventLoopSync();

			void closeAllWindows();

		private:
			Jargon::SmartPointer<Window> m_mainWindow;
			std::vector<Jargon::SmartPointer<Window> > m_windows;

			JARGON_DISABLE_COPY(WindowGroup);
	};

}

#endif
