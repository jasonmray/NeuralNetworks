
#ifndef J_MNIST_IMAGESET_H
#define J_MNIST_IMAGESET_H

#include <Eigen/Eigen>

#include <cassert>
#include <cstdint>
#include <vector>


namespace J{
namespace Mnist{

	typedef Eigen::Matrix<uint8_t, -1, 1> VectorXByte;

	class ImageSet {
		public:
			struct Header {
				uint32_t magicNumber;
				uint32_t imageCount;
				uint32_t rowCount;
				uint32_t columnCount;
			};

			struct Image {
				static void PrintImage(const ImageSet::Image & image);

				static const uint8_t InvalidLabel;

				Image(const Header &header);

				uint8_t operator()(size_t row, size_t column) const;
				Eigen::VectorXd getLabelVector() const;

				const Header &header;
				VectorXByte pixels;
				uint8_t label;
			};

			Header header;
			std::vector<Image> images;

			bool load(const char * filename);
	};
}
}
#endif
