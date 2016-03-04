
#ifndef J_CONTROLLER_H
#define J_CONTROLLER_H

#include "Display.h"
#include "Jargon/Macros.h"

namespace J{

	class Controller {
		public:
			Controller(Display & display);
			~Controller();

			void runAsync();
			void stop();
			void waitForCompletion();

		private:
			void process();

			sf::Thread m_processThread;
			J::Display & m_display;
			bool m_shouldStop;

			JARGON_DISABLE_COPY(Controller);
	};


}

#endif
