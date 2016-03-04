
#include "NetworkLayer.h"

#include "JsonUtil.h"


namespace J{


	DefaultNetworkLayerInitializationStrategy::DefaultNetworkLayerInitializationStrategy(RandomGenerator & randomGenerator):
		m_randomGenerator(randomGenerator)
	{
	}

	void DefaultNetworkLayerInitializationStrategy::initialize(Eigen::MatrixXd & weights, Eigen::VectorXd & biases){
		m_randomGenerator.fill(biases);

		m_randomGenerator.fill(weights);
		weights /= std::sqrt((double)weights.cols());
	}



	NetworkLayer::NetworkLayer(unsigned int inputCount, unsigned int nodeCount){
		m_weights.resize((Eigen::DenseIndex)nodeCount, (Eigen::DenseIndex)inputCount);
		m_biases.resize((Eigen::DenseIndex)nodeCount);
	}

	NetworkLayer::NetworkLayer(){
	}

	NetworkLayer::~NetworkLayer(){
	}

	void NetworkLayer::initialize(NetworkLayerInitializationStrategy & strategy){
		strategy.initialize(m_weights, m_biases);
	}

	Eigen::VectorXd NetworkLayer::feedForward(const Eigen::VectorXd & inputs) const{
		return m_weights * inputs + m_biases;
	}

	Eigen::VectorXd NetworkLayer::backProp(const Eigen::VectorXd & delta) const{
		return m_weights.transpose() * delta;
	}

	bool NetworkLayer::restore(const Dropbox::Json & contents){
		auto & jsonObject(contents.object_items());

		auto & biases = Jargon::StlUtilities::GetOrDefault(jsonObject, std::string("biases"), Dropbox::Json()).array_items();
		m_biases = JsonUtil::parseVector(biases);

		auto & weights = Jargon::StlUtilities::GetOrDefault(jsonObject, std::string("weights"), Dropbox::Json()).array_items();
		m_weights = JsonUtil::parseMatrix(weights);

		return true;
	}

	Dropbox::Json::object NetworkLayer::serialize() const{
		return Dropbox::Json::object {
			{ "biases", JsonUtil::jsonify(m_biases) },
			{ "weights", JsonUtil::jsonify(m_weights) }
		};
	}


}
