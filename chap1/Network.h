
#ifndef J_CHAP1_NETWORK_H
#define J_CHAP1_NETWORK_H

#include "DataSet.h"
#include "NetworkLayer.h"
#include "RandomGenerator.h"
#include "Math.h"
#include "DropboxJson.h"

#include "Eigen/Eigen"
#include "Jargon/Advanced/TaskQueue.h"

#include <vector>


namespace J{
	class NetworkRenderer;

	class Network{
		public:
			Network(const std::vector<size_t> layerSizes);

			~Network();

			int recognize(const Mnist::ImageSet::Image & image, NetworkRenderer * networkRenderer = NULL);
			Eigen::VectorXd feedForward(const Mnist::VectorXByte & input, NetworkRenderer * networkRenderer = NULL) const;
			Eigen::VectorXd feedForward(const Eigen::VectorXd & input, NetworkRenderer * networkRenderer = NULL) const;

			struct SgdOptions {
				size_t miniBatchSize;    // number of items to process together.
				double eta;              // the increment size of the descent
				bool runTests;           // run the tests
				Jargon::Advanced::TaskQueue * taskQueue;
				NetworkRenderer * networkRenderer;
			};

			void runStochasticGradientDescent(const SgdOptions & options, const Mnist::DataSet & dataset, bool & stopFlag);

			size_t runTests(const Mnist::DataSet & dataset, NetworkRenderer * networkRenderer = NULL);

			bool restore(const std::string & state);
			bool serialize(std::string * out) const;
			void reset();

		private:
			class BackpropTask;

			static const char * Schema;

			RandomGenerator m_randomGenerator;
			std::vector<size_t> m_layerSizes;

			typedef NetworkLayer LayerType;
			std::vector<LayerType> m_layers;

			J::Math::ReLUActivation m_activationFunc;
			J::Math::QuadraticCost m_costFunc;

			void updateMiniBatch(const SgdOptions & options, const Mnist::DataSet::ImageList & miniBatch);
			void updateMiniBatchParallel(const SgdOptions & options, const Mnist::DataSet::ImageList & miniBatch, Jargon::Advanced::TaskQueue & taskQueue);
			void backprop(const SgdOptions & options, const Mnist::ImageSet::Image & image, std::vector<Eigen::MatrixXd> & nablaWeights, std::vector<Eigen::VectorXd> & nablaBiases) const;
			void initializeParameterVectors(std::vector<Eigen::MatrixXd> & weights, std::vector<Eigen::VectorXd> & biases);

			bool restoreState(const Dropbox::Json & state);

			friend class NetworkRenderer;
	};

}
#endif
