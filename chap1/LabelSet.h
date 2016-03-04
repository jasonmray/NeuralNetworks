
#ifndef J_MNIST_LABELSET_H
#define J_MNIST_LABELSET_H

#include <cassert>
#include <cstdint>
#include <vector>

#include "ImageSet.h"


namespace J{
namespace Mnist{

	class LabelSet {
	public:
		typedef uint8_t Label;

		struct Header {
			uint32_t magicNumber;
			uint32_t labelCount;
		};

		Header header;
		std::vector<Label> labels;

		bool load(const char * filename, ImageSet * imageSet);
	};

}
}
#endif
