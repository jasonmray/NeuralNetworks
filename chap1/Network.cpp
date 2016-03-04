
#include "DropboxJson.h"
#include "Network.h"
#include "NetworkRenderer.h"
#include "Math.h"
#include "EigenUtil.h"
#include "JsonUtil.h"

#include "Jargon/Functional.h"
#include "Jargon/StlUtilities.h"
#include "Jargon/Advanced/CollectingMutex.h"

#include "Poco/Stopwatch.h"

#include <algorithm>
#include <cassert>
#include <cstdio>


namespace J {


	int findIndexOfMaxComponent(const Eigen::VectorXd & v) {
		Eigen::VectorXd::Index maxIndex = 0;
		for (Eigen::VectorXd::Index i = 1; i < v.size(); i++) {
			if (v[i] > v[maxIndex]) {
				maxIndex = i;
			}
		}
		if (v[maxIndex] == 0) {
			return -1;
		}
		return (int)maxIndex;
	}

	bool checkOutput(const Eigen::VectorXd & vectorOutput, const Mnist::LabelSet::Label & label) {
		int indexOfMax = findIndexOfMaxComponent(vectorOutput);
		return indexOfMax != -1 && indexOfMax == label;
	}


	const char * Network::Schema = "jargon:neuralnet:network:v1";

	Network::Network(const std::vector<size_t> layerSizes) :
		m_layerSizes(layerSizes)
	{
		assert(layerSizes.size() > 1);

		for (size_t i = 1; i < layerSizes.size(); i++) {
			m_layers.push_back(LayerType((unsigned int)layerSizes[i-1], (unsigned int)layerSizes[i]));
		}

		DefaultNetworkLayerInitializationStrategy layerInitializer(m_randomGenerator);
		for( LayerType & layer : m_layers ){
			layer.initialize(layerInitializer);
		}
	}

	Network::~Network(){
	}

	int Network::recognize(const Mnist::ImageSet::Image & image, NetworkRenderer * networkRenderer){
		Eigen::VectorXd result(feedForward(image.pixels, networkRenderer));
		result = J::Math::softmax(result);
		return findIndexOfMaxComponent(result);
	}

	Eigen::VectorXd Network::feedForward(const Eigen::VectorXd & input, NetworkRenderer * networkRenderer) const{
		Eigen::VectorXd result(input);

		for( const LayerType & layer : m_layers ){
			result = m_activationFunc.forward(layer.feedForward(result));
		}


		if( networkRenderer != NULL ){
			std::vector<std::vector<Eigen::VectorXd> > weightedImages(m_layers.size());

			for( Eigen::MatrixXd::Index nodeIndex = 0; nodeIndex < m_layers[0].m_weights.rows(); ++nodeIndex ){
				weightedImages[0].resize(m_layers[0].m_weights.rows());
				weightedImages[0][nodeIndex] = m_layers[0].m_weights.row(nodeIndex);
				networkRenderer->showWeightedImage(weightedImages[0][nodeIndex], 28, 28, 1, (size_t)nodeIndex);
			}

			/*
			for( size_t layerIndex = 1; layerIndex < m_layers.size(); layerIndex++ ){
				for( Eigen::MatrixXd::Index nodeIndex = 0; nodeIndex < m_layers[layerIndex].m_weights.rows(); ++nodeIndex ){
					Eigen::VectorXd weightedImage(m_layers[layerIndex].m_weights.row(nodeIndex));
					networkRenderer->showWeightedImage(weightedImage, 28, 28, layerIndex+1, (size_t)nodeIndex);
				}
			}
			*/

			for( Eigen::MatrixXd::Index nodeIndex1 = 0; nodeIndex1 < m_layers[1].m_weights.rows(); ++nodeIndex1 ){
				Eigen::VectorXd weightedImage(m_layers[0].m_weights.row(nodeIndex1));
				weightedImage.setZero();
				for( Eigen::MatrixXd::Index nodeIndex2 = 0; nodeIndex2 < m_layers[0].m_weights.rows(); ++nodeIndex2 ){
					Eigen::VectorXd componentImage = m_layers[0].m_weights.row(nodeIndex2) * m_layers[1].m_weights.row(nodeIndex1)[nodeIndex2];
					weightedImage += componentImage;
				}
				networkRenderer->showWeightedImage(weightedImage, 28, 28, 2, nodeIndex1);
			}

		}

		return result;
	}

	Eigen::VectorXd Network::feedForward(const Mnist::VectorXByte & input, NetworkRenderer * networkRenderer) const {
		Eigen::VectorXd normedInput(input.cast<double>() / 255.0);
		return feedForward(normedInput, networkRenderer);
	}

	void Network::backprop(const SgdOptions & options, const Mnist::ImageSet::Image & image, std::vector<Eigen::MatrixXd> & deltaNablaWeights, std::vector<Eigen::VectorXd> & deltaNablaBiases) const{
		std::vector<Eigen::VectorXd> activations(m_layers.size());
		std::vector<Eigen::VectorXd> weightedSums(m_layers.size());

		Eigen::VectorXd input = image.pixels.cast<double>() / 255.0;
		Eigen::VectorXd * currentResult = &input;

		for(size_t i = 0; i < m_layers.size(); i++ ){
			weightedSums[i] = m_layers[i].feedForward(*currentResult);
			if( i == m_layers.size() - 1 ){
				activations[i] = weightedSums[i];
			}else{
				activations[i] = m_activationFunc.forward(weightedSums[i]);
			}
			currentResult = &activations[i];
		}

		Eigen::VectorXd delta = m_costFunc.computeCostGradient(activations.back(), image.getLabelVector());//.cwiseProduct(m_activationFunc.derivative(weightedSums.back()));
		deltaNablaBiases.back() = delta;
		deltaNablaWeights.back() = delta * (activations.end() - 2)->transpose();

		for(size_t layerIndex = m_layers.size() - 2;  ; layerIndex--){
			const Eigen::VectorXd & z(weightedSums[layerIndex]);
			Eigen::VectorXd spz = m_activationFunc.derivative(z);

			Eigen::VectorXd * previousActivations;
			if( layerIndex == 0 ){
				previousActivations = &input;
			}else{
				previousActivations = &activations[layerIndex-1];
			}

			delta = m_layers[layerIndex+1].backProp(delta).cwiseProduct(spz);
			deltaNablaBiases[layerIndex] = delta;
			deltaNablaWeights[layerIndex] = delta * previousActivations->transpose();

			if( layerIndex == 0 ){
				break;
			}
		}
	}

	void Network::initializeParameterVectors(std::vector<Eigen::MatrixXd> & weights, std::vector<Eigen::VectorXd> & biases) {
		weights.resize(m_layers.size());
		for (size_t i = 0; i < m_layers.size(); i++) {
			weights.at(i).resizeLike(m_layers[i].m_weights);
			weights.at(i).setZero();

		}
		biases.resize(m_layers.size());
		for (size_t i = 0; i < m_layers.size(); i++) {
			biases.at(i).resizeLike(m_layers[i].m_biases);
			biases.at(i).setZero();
		}
	}

	class Network::BackpropTask : public Jargon::Advanced::Task{
		public:
			BackpropTask(Network & network, const SgdOptions & options, const Mnist::ImageSet::Image & image, Jargon::Advanced::CollectingMutex & collectingMutex):
				m_network(network),
				m_options(options),
				m_image(image),
				m_collectingMutex(collectingMutex)
			{
			}

			void run(){
				m_network.initializeParameterVectors(deltaNablaWeights, deltaNablaBiases);
				m_network.backprop(m_options, m_image, deltaNablaWeights, deltaNablaBiases);
				m_collectingMutex.signalOne();
			}

			void collect(std::vector<Eigen::MatrixXd> & nablaWeights, std::vector<Eigen::VectorXd> nablaBiases){
				for( size_t i = 0; i < nablaWeights.size(); i++ ){
					nablaWeights[i] += deltaNablaWeights[i];
				}
				for( size_t i = 0; i < nablaBiases.size(); i++ ){
					nablaBiases[i] += deltaNablaBiases[i];
				}
			}

		private:
			Network & m_network;
			const SgdOptions & m_options;
			const Mnist::ImageSet::Image & m_image;
			Jargon::Advanced::CollectingMutex & m_collectingMutex;

			std::vector<Eigen::MatrixXd> deltaNablaWeights;
			std::vector<Eigen::VectorXd> deltaNablaBiases;
	};

	void Network::updateMiniBatchParallel(const SgdOptions & options, const Mnist::DataSet::ImageList & miniBatch, Jargon::Advanced::TaskQueue & taskQueue) {
		Jargon::Advanced::CollectingMutex collectionMutex((unsigned int)miniBatch.size());

		std::vector<Jargon::SmartPointer<BackpropTask> > tasks;
		tasks.reserve(miniBatch.size());

		for(auto & image : miniBatch) {
			Jargon::SmartPointer<BackpropTask> task(new BackpropTask(*this, options, *image, collectionMutex));
			tasks.push_back(task);
		}

		for( auto & task : tasks ){
			taskQueue.enqueue(task);
		}
		collectionMutex.wait();

		std::vector<Eigen::MatrixXd> nablaWeights;
		std::vector<Eigen::VectorXd> nablaBiases;
		initializeParameterVectors(nablaWeights, nablaBiases);
		for( auto & task : tasks ){
			task->collect(nablaWeights, nablaBiases);
		}

		double multiplier = options.eta / options.miniBatchSize;
		for (size_t i = 0; i < m_layers.size(); i++) {
			m_layers[i].m_weights -= nablaWeights[i] * multiplier;
			m_layers[i].m_biases -= nablaBiases[i] * multiplier;
		}
	}

	void Network::updateMiniBatch(const SgdOptions & options, const Mnist::DataSet::ImageList & miniBatch) {
		std::vector<Eigen::MatrixXd> nablaWeights;
		std::vector<Eigen::VectorXd> nablaBiases;
		initializeParameterVectors(nablaWeights, nablaBiases);

		for(auto & image : miniBatch) {
			std::vector<Eigen::MatrixXd> deltaNablaWeights;
			std::vector<Eigen::VectorXd> deltaNablaBiases;
			initializeParameterVectors(deltaNablaWeights, deltaNablaBiases);

			backprop(options, *image, deltaNablaWeights, deltaNablaBiases);


			for (size_t i = 0; i < nablaWeights.size(); i++) {
				nablaWeights[i] += deltaNablaWeights[i];
			}
			for (size_t i = 0; i < nablaBiases.size(); i++) {
				nablaBiases[i] += deltaNablaBiases[i];
			}

			if( options.networkRenderer != nullptr ){
				options.networkRenderer->showBackprop(*image, deltaNablaWeights, deltaNablaBiases);
			}
		}

		double multiplier = options.eta / options.miniBatchSize;
		for (size_t i = 0; i < m_layers.size(); i++) {
			m_layers[i].m_weights -= nablaWeights[i] * multiplier;
			m_layers[i].m_biases -= nablaBiases[i] * multiplier;
		}

		if( options.networkRenderer != nullptr ){
			for( Eigen::MatrixXd::Index nodeIndex = 0; nodeIndex < m_layers[0].m_weights.rows(); ++nodeIndex ){
				Eigen::VectorXd weightedImage(m_layers[0].m_weights.row(nodeIndex));
				options.networkRenderer->showWeightedImage(weightedImage, 28, 28, 1, (size_t)nodeIndex);
			}
		}
	}

	size_t Network::runTests(const Mnist::DataSet & dataset, NetworkRenderer * networkRenderer) {
		auto testingImages(dataset.getTestingImages());

		size_t numCorrect = 0;
		for (auto & image : testingImages) {
			Eigen::VectorXd output(feedForward(image.pixels, networkRenderer));
			if (checkOutput(output, image.label)) {
				++numCorrect;
			}
		}
		return numCorrect;
	}

	void Network::runStochasticGradientDescent(const SgdOptions & options, const Mnist::DataSet & dataset, bool & stopFlag) {
		Mnist::DataSet::ImageList trainingImages(dataset.getTrainingImageRefs());

		Poco::Stopwatch stopwatch;
		stopwatch.restart();
		for (size_t startIndex = 0; stopFlag == false && startIndex + options.miniBatchSize <= trainingImages.size(); startIndex += options.miniBatchSize) {
			Mnist::DataSet::ImageList::const_iterator batchStart(trainingImages.cbegin() + startIndex);
			Mnist::DataSet::ImageList::const_iterator batchEnd(batchStart + options.miniBatchSize);
			Mnist::DataSet::ImageList miniBatch(batchStart, batchEnd);

			if( options.taskQueue != NULL ){
				updateMiniBatchParallel(options, miniBatch, *options.taskQueue);
			}else{
				updateMiniBatch(options, miniBatch);
			}
		}
		stopwatch.stop();
		printf("time : %f ms\n", stopwatch.elapsed() / 1000.f);


		if (stopFlag == false && options.runTests) {
			size_t numCorrect = runTests(dataset);
			printf("Num Correct %zd\n", numCorrect);
		}
	}

	void Network::reset(){
		m_layerSizes.clear();
		m_layers.clear();
	}

	bool Network::restoreState(const Dropbox::Json & state){

		auto & contents(state.object_items());

		{
			std::string schema = Jargon::StlUtilities::GetOrDefault(contents, std::string("schema"), Dropbox::Json()).string_value();
			if( schema != Schema ){
				return false;
			}
		}

		{
			auto & layerSizes = Jargon::StlUtilities::GetOrDefault(contents, std::string("layerSizes"), Dropbox::Json()).array_items();
			if( layerSizes.size() < 2 ){
				return false;
			}
			Jargon::Functional::map(layerSizes, m_layerSizes, [](const Dropbox::Json & val){return (size_t)val.int_value(); });
		}
		{
			auto & layers = Jargon::StlUtilities::GetOrDefault(contents, std::string("layers"), Dropbox::Json()).array_items();
			if( layers.size() != m_layerSizes.size()-1 ){
				return false;
			}

			m_layers.resize(layers.size());
			if( m_layers.size() != m_layerSizes.size() - 1 ){
				return false;
			}

			for( size_t i = 0; i < layers.size(); i++ ){
				if( !m_layers[i].restore(layers[i]) ){
					return false;
				}
			}
		}

		return true;
	}

	bool Network::restore(const std::string & state){

		reset();

		std::string error;
		Dropbox::Json body = Dropbox::Json::parse(state, error);
		if( error.size() > 0 ){
			reset();
			return false;
		}

		if( !restoreState(body) ){
			reset();
			return false;
		}
		return true;
	}

	bool Network::serialize(std::string * out) const{

		Dropbox::Json body = Dropbox::Json::object {
			{ "schema", Schema },
			{ "layerSizes", JsonUtil::jsonify(m_layerSizes) },
			{ "layers", JsonUtil::jsonify(m_layers.begin(), m_layers.end()) }
		};

		body.dump(*out);
		return true;
	}
}
