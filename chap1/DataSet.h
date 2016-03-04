
#ifndef J_MNIST_DATASET_H
#define J_MNIST_DATASET_H

#include "ImageSet.h"
#include "LabelSet.h"

#include "Jargon/IteratorRange.h"


namespace J{
namespace Mnist{

	class DataSet{
		public:
			typedef std::vector<ImageSet::Image>::iterator ImageIterator;
			typedef std::vector<ImageSet::Image>::const_iterator ConstImageIterator;
			typedef std::vector<const ImageSet::Image *> ImageList;

			DataSet();
			~DataSet();

			// load the mnist data
			bool load(const std::string & path);

			// return a list of pointers to the training images
			std::vector<const ImageSet::Image *> getTrainingImageRefs() const;

			// return begin and end iterators for the image sets
			Jargon::IteratorRange<ConstImageIterator> getTrainingImages() const;
			Jargon::IteratorRange<ConstImageIterator> getValidationImages() const;
			Jargon::IteratorRange<ConstImageIterator> getTestingImages() const;

		private:
			static const size_t MnistTrainingImageCount = 50000;

			bool loadTrainingImages(const std::string & path);
			bool loadTestingImages(const std::string & path);

			ImageSet trainingImages;
			ImageSet testingImages;
	};

}
}

#endif
