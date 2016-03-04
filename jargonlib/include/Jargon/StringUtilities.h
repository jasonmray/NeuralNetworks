
#ifndef JARGON_STRINGUTILITIES_H
#define JARGON_STRINGUTILITIES_H

#include <string>
#include <cstdarg>

namespace Jargon{
namespace StringUtilities{

	std::string format(const char * formatString, ...);
	std::string formatVarArgs(const char * formatString, va_list args);
}
}

#endif
