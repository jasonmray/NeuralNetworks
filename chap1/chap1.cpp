
#include "Jargon/System/WindowsMemoryLeakHelpers.h"

#include "Display.h"
#include "Controller.h"

#include "Jargon/SmartPointer.h"
#include "Jargon/Macros.h"

#include "Eigen/Eigen"
#include "SFML/System.hpp"

#include <cstdio>
#include <string>
#include <cstdint>
#include <cassert>
#include <vector>
#include <memory>

JARGON_WINDOWS_BREAK_AT_ALLOCATION_NUMBER(-1);

int main(int argc, char ** argv){

	J::Display display;
	J::Controller controller(display);
	controller.runAsync();
	display.showSync();
	controller.stop();
	controller.waitForCompletion();

	JARGON_WINDOWS_DUMP_MEMORY_LEAKS();
	return 0;
}
