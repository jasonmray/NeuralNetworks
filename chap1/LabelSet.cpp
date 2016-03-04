
#include "LabelSet.h"

#include <fstream>

#include "Poco/BinaryReader.h"
#include "Poco/InflatingStream.h"

namespace J {
namespace Mnist{

	bool readHeader(LabelSet::Header * header, Poco::BinaryReader & source) {
		source >> header->magicNumber;
		source >> header->labelCount;

		if (header->magicNumber != 2049) {
			return false;
		}

		assert(header->labelCount <= 60000);

		return source.good();
	}

	bool LabelSet::load(const char * filename, ImageSet * imageSet) {
		std::ifstream inputStream(filename, std::ios::binary);
		Poco::InflatingInputStream inflateStream(inputStream, Poco::InflatingStreamBuf::STREAM_GZIP);
		Poco::BinaryReader source(inflateStream, Poco::BinaryReader::BIG_ENDIAN_BYTE_ORDER);

		if (!readHeader(&header, source)) {
			return false;
		}

		if (header.labelCount != imageSet->images.size()) {
			return false;
		}
		for (uint32_t i = 0; i < header.labelCount; i++) {
			source >> imageSet->images[i].label;
		}

		return source.good();
	}

}
}
