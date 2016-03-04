
#include "Controller.h"
#include "DataSet.h"
#include "Network.h"
#include "NetworkRenderer.h"

#include "Jargon/System/Utilities.h"
#include "Jargon/Advanced/TaskQueue.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"

#include <fstream>

namespace J{

	Controller::Controller(Display & display) :
		m_processThread(&Controller::process, this),
		m_display(display)
	{
	}

	Controller::~Controller(){
	}

	void Controller::runAsync(){
		m_shouldStop = false;
		m_processThread.launch();
	}

	void Controller::stop(){
		m_shouldStop = true;
	}

	void Controller::waitForCompletion(){
		m_processThread.wait();
	}

	void Controller::process(){
		printf("Loading data.. ");
		J::Mnist::DataSet dataSet;
		if (!dataSet.load("..\\..\\data")) {
			printf("failed!\n");
			return;
		}
		printf("done\n");

		std::vector<size_t> layerSizes{
			(size_t)dataSet.getTestingImages().begin()->pixels.size(),
			28,
			10
		};
		assert(layerSizes[0] == 784);

		Jargon::Advanced::TaskQueue taskQueue(Jargon::System::getHardwareConcurrencyCount());
		taskQueue.start();

		J::Network network(layerSizes);
		if(0)
		{
			std::ifstream inputStream("..\\Release\\state.json", std::ios::binary);
			std::stringstream buffer;
			buffer << inputStream.rdbuf();
			std::string contents(buffer.str());
			network.restore(contents);
		}

		J::NetworkRenderer networkRenderer(m_display, network);

		J::Network::SgdOptions options;
		options.eta = 0.03;
		options.miniBatchSize = 20;
		options.runTests = true;
		options.taskQueue = &taskQueue;
		options.networkRenderer = NULL;//&networkRenderer;

		Eigen::VectorXd blankInput;
		blankInput.resize(28*28);
		blankInput.setOnes();

		int epochCount = 20;
		for( int epochNum = 0; epochNum < epochCount && !m_shouldStop; epochNum++ ){
			network.runStochasticGradientDescent(options, dataSet, m_shouldStop);
			printf("Epoch %d complete\n\n", epochNum);
			networkRenderer.updateNetworkStructure();
			network.feedForward(blankInput, &networkRenderer);
		}

		{
			std::string state;
			network.serialize(&state);

			std::ofstream outputStream("..\\Release\\state_new.json", std::ios::binary);
			Poco::BinaryWriter writer(outputStream);
			writer.writeRaw(state);
		}

		for( auto & image : dataSet.getTestingImages() ){
			if( m_shouldStop ){
				break;
			}

			networkRenderer.updateInputImage(image);
			int result = network.recognize(image, &networkRenderer);
			networkRenderer.updateLabel(result, result == image.label);
			if( result != image.label ){
				Mnist::ImageSet::Image::PrintImage(image);
			}
			Jargon::System::sleep(500);
		}

		taskQueue.stop(Jargon::Advanced::TaskQueue::StopMode_IgnoreQueuedTasks);
	}

}
