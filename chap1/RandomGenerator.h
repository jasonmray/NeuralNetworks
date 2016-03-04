
#ifndef J_RANDOMGENERATOR_H
#define J_RANDOMGENERATOR_H

#include <random>

#include "Eigen/Eigen"


namespace J{

	class RandomGenerator{
		public:
			RandomGenerator();

			double next();
			double operator()();
			void seed(unsigned int s);
			void fill(Eigen::VectorXd & vector);
			void fill(Eigen::MatrixXd & matrix);
		private:
			std::normal_distribution<double> distribution;
			std::default_random_engine generator;
	};

}

#endif
