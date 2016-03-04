
#include "Math.h"

namespace J {
namespace Math {

	double Sigmoid::operator()(double z) const {
		return 1.0 / (1.0 + std::exp(-z));
	}

	double SigmoidPrime::operator()(double z) const {
		Sigmoid sigmoid;
		return sigmoid(z)*(1.0 - sigmoid(z));
	}


	double ReLU::operator()(double z) const {
		return std::max(0., z);
	}

	double ReLUPrime::operator()(double z) const {
		return z > 0. ? 1. : 0.;
	}


	LeakyReLU::LeakyReLU(double slopeWhenNegative):
		m_slopeWhenNegative(slopeWhenNegative)
	{
	}

	double LeakyReLU::operator()(double z) const {
		if( z < 0. ){
			return m_slopeWhenNegative * z;
		}else{
			return z;
		}
	}


	LeakyReLUPrime::LeakyReLUPrime(double slopeWhenNegative):
		m_slopeWhenNegative(slopeWhenNegative)
	{
	}

	double LeakyReLUPrime::operator()(double z) const {
		if( z > 0. ){
			return 1;
		}else{
			return m_slopeWhenNegative;
		}
	}

	double Tanh::operator()(double z) const{
		return (std::tanh(z)+1) / 2.;
	}

	double TanhPrime::operator()(double z) const{
		return 1.0/(std::cosh(z) * std::cosh(z)) / 2.;
	}


	LeakyReLUActivation::LeakyReLUActivation(double slopeWhenNegative) :
		m_forward(slopeWhenNegative),
		m_derivative(slopeWhenNegative),
		DifferentiableFunction(m_forward, m_derivative)
	{
	}


	Eigen::VectorXd softmax(const Eigen::VectorXd & v){
		Eigen::VectorXd result(v);
		double denominator = 0;
		for (Eigen::VectorXd::Index i = 0; i < result.size(); i++) {
			denominator += std::exp(result[i]);
		}
		for (Eigen::VectorXd::Index i = 0; i < result.size(); i++) {
			result[i] = std::exp(result[i]) / denominator;
		}
		return result;
	}
}
}