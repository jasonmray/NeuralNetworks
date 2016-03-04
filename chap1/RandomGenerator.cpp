
#include "RandomGenerator.h"
#include "Jargon/Macros.h"

namespace J{

	RandomGenerator::RandomGenerator():
		distribution(0.0, 1.0)
	{
#ifdef JARGON_DEBUG
		seed(0);
#endif
	}

	void RandomGenerator::seed(unsigned int s){
		generator.seed(s);
	}

	double RandomGenerator::next() {
		return distribution(generator);
	}
	double RandomGenerator::operator()() {
		return next();
	}
	void RandomGenerator::fill(Eigen::VectorXd & vector) {
		for (Eigen::VectorXd::Index i = 0; i < vector.size(); i++) {
			vector[i] = next();
		}
	}

	void RandomGenerator::fill(Eigen::MatrixXd & matrix) {
		for (Eigen::MatrixXd::Index row = 0; row < matrix.rows(); row++) {
			for (Eigen::MatrixXd::Index col = 0; col < matrix.cols(); col++) {
				matrix(row, col) = next();
			}
		}
	}

}
