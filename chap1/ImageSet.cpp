
#include "ImageSet.h"

#include <fstream>

#include "Poco/BinaryReader.h"
#include "Poco/BinaryReader.h"
#include "Poco/InflatingStream.h"

namespace J {
namespace Mnist{

	bool readHeader(ImageSet::Header * header, Poco::BinaryReader & source) {
		source >> header->magicNumber;
		source >> header->imageCount;
		source >> header->rowCount;
		source >> header->columnCount;

		if (header->magicNumber != 2051) {
			return false;
		}

		assert(header->imageCount <= 60000);
		assert(header->rowCount == 28);
			assert(header->columnCount == 28);

		return source.good();
	}

	bool readImage(ImageSet::Image * image, Poco::BinaryReader & source) {

		image->pixels.resize(image->header.rowCount * image->header.columnCount);

		std::vector<uint8_t> buf(image->header.rowCount * image->header.columnCount);
		source.readRaw((char *)&buf[0], buf.size());

		size_t index = 0;
		for (uint32_t row = 0; row < image->header.rowCount; row++) {
			//source.readRaw((char *)&image->pixels[0], image->header.rowCount);
			for (uint32_t column = 0; column < image->header.columnCount; column++) {
				image->pixels[index] = buf[index];
				index++;
			}
		}
		return source.good();
	}

	void ImageSet::Image::PrintImage(const ImageSet::Image & image) {
		printf("\n\n");
		size_t index = 0;
		for (uint32_t row = 0; row < image.header.rowCount; row++) {
			for (uint32_t column = 0; column < image.header.columnCount; column++) {
				printf("%s", image.pixels[index] < 128 ? "0" : "1");
				index++;
			}
			printf("\n");
		}
		if (image.label != Image::InvalidLabel) {
			printf("[%u]\n", image.label);
		}
		printf("\n");
	}

	bool ImageSet::load(const char * filename) {
		std::ifstream inputStream(filename, std::ios::binary);
		Poco::InflatingInputStream inflateStream(inputStream, Poco::InflatingStreamBuf::STREAM_GZIP);
		Poco::BinaryReader source(inflateStream, Poco::BinaryReader::BIG_ENDIAN_BYTE_ORDER);

		if (!readHeader(&header, source)) {
			return false;
		}

		images.resize(header.imageCount, Image(header));
		for (uint32_t image = 0; image < header.imageCount; image++) {
			if (! readImage(&images[image], source) ) {
				return false;
			}
		}

		return true;
	}


	const uint8_t ImageSet::Image::InvalidLabel = (uint8_t)-1;

	ImageSet::Image::Image(const Header &header) :
		header(header),
		label(InvalidLabel)
	{
	}

	uint8_t ImageSet::Image::operator()(size_t row, size_t column) const {
		return pixels[row * header.columnCount + column];
	}

	Eigen::VectorXd ImageSet::Image::getLabelVector() const {
		Eigen::VectorXd labelVector(10);
		labelVector.setZero();
		if( label != InvalidLabel ){
			labelVector[label] = 1.0;
		}
		return labelVector;
	}
}
}