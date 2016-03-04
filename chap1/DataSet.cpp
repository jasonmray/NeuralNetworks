
#include "DataSet.h"

namespace J{
namespace Mnist{

	DataSet::DataSet(){
	}

	DataSet::~DataSet(){
	}

	bool DataSet::load(const std::string & path) {
		return loadTrainingImages(path) && loadTestingImages(path);
	}

	bool DataSet::loadTrainingImages(const std::string & path) {
		if (!trainingImages.load( (path + "\\train-images-idx3-ubyte.gz").c_str() )) {
			return false;
		}

		LabelSet trainingLabels;
		if (!trainingLabels.load( (path + "\\train-labels-idx1-ubyte.gz").c_str(), &trainingImages)) {
			return false;
		}

		return true;
	}

	bool DataSet::loadTestingImages(const std::string & path) {
		if (!testingImages.load( (path + "\\t10k-images-idx3-ubyte.gz").c_str() )) {
			return false;
		}

		LabelSet testingLabels;
		if (!testingLabels.load( (path + "\\t10k-labels-idx1-ubyte.gz").c_str(), &testingImages)) {
			return false;
		}

		return true;
	}


	DataSet::ImageList DataSet::getTrainingImageRefs() const {
		ImageList imageRefs;
		imageRefs.reserve(MnistTrainingImageCount);
		for (size_t i = 0; i < MnistTrainingImageCount; i++) {
			imageRefs.push_back(&trainingImages.images[i]);
		}
		return imageRefs;
	}

	Jargon::IteratorRange<DataSet::ConstImageIterator> DataSet::getTrainingImages()  const {
		return Jargon::IteratorRange<DataSet::ConstImageIterator>(
			trainingImages.images.begin(),
			trainingImages.images.begin() + 50000
		);
	}

	Jargon::IteratorRange<DataSet::ConstImageIterator> DataSet::getValidationImages() const {
		return Jargon::IteratorRange<DataSet::ConstImageIterator>(
			trainingImages.images.begin() + 50000,
			trainingImages.images.end()
		);
	}

	Jargon::IteratorRange<DataSet::ConstImageIterator> DataSet::getTestingImages()  const {
		return Jargon::IteratorRange<DataSet::ConstImageIterator>(
			testingImages.images.begin(),
			testingImages.images.end()
		);
	}

}
}