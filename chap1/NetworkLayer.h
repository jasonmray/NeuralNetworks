
#ifndef J_NETWORKLAYER_H
#define J_NETWORKLAYER_H

#include "RandomGenerator.h"
#include "Math.h"

#include "Eigen/Eigen"
#include "DropboxJson.h"
#include "Jargon/StlUtilities.h"


namespace J{

	class NetworkLayerInitializationStrategy{
		public:
			virtual ~NetworkLayerInitializationStrategy() = default;
			virtual void initialize(Eigen::MatrixXd & weights, Eigen::VectorXd & biases) = 0;
	};

	class DefaultNetworkLayerInitializationStrategy : public NetworkLayerInitializationStrategy{
		public:
			DefaultNetworkLayerInitializationStrategy(RandomGenerator & randomGenerator);

			void initialize(Eigen::MatrixXd & weights, Eigen::VectorXd & biases);

		private:
			RandomGenerator & m_randomGenerator;
	};


	class NetworkLayer{
		public:
			class Delta{
				Eigen::MatrixXd m_deltaWeights;
				Eigen::VectorXd m_deltaBiases;
			};

			NetworkLayer();
			NetworkLayer(unsigned int inputCount, unsigned int nodeCount);
			~NetworkLayer();

			void initialize(NetworkLayerInitializationStrategy & strategy);

			Eigen::VectorXd feedForward(const Eigen::VectorXd & inputs) const;
			Eigen::VectorXd backProp(const Eigen::VectorXd & delta) const;

			bool restore(const Dropbox::Json & contents);
			Dropbox::Json::object serialize() const;

		public:
			Eigen::MatrixXd m_weights;
			Eigen::VectorXd m_biases;
	};
}

#endif
