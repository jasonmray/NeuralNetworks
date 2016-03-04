
#include "Jargon/Null.h"
#include "Jargon/StringUtilities.h"

namespace Jargon{
namespace StringUtilities{

	std::string format(const char * formatString, ...){
		va_list args;
		va_start(args, formatString);
		std::string s(formatVarArgs(formatString, args));
		va_end(args);
		return s;
	}

	std::string formatVarArgs(const char * formatString, va_list args){
		size_t bufferLength = vsnprintf(NULL, 0, formatString, args) + 1;
		char * buffer = (char *)alloca(bufferLength * sizeof(char));
		vsnprintf(buffer, bufferLength, formatString, args);

		return std::string(buffer, 0, bufferLength);
	}
}
}
