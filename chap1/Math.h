
#ifndef J_MATH_H
#define J_MATH_H

#include "Eigen/Eigen"
#include "Jargon/Functional.h"

namespace J{
namespace Math {

	template<class T>
	class VectorizableFunction{
		public:
			typedef Eigen::Matrix<T, -1, 1> VectorType;

			virtual ~VectorizableFunction() = default;
			virtual T operator()(T v) const = 0;
			virtual VectorType operator()(const VectorType & v) const{
				VectorType result(v);
				for (VectorType::Index i = 0; i < result.size(); i++) {
					result[i] = (*this)(result[i]);
				}
				return result;
			}
	};

	template<class T, class F>
	class VectorizableFunctionHelper : public VectorizableFunction<T>{
		public:
			virtual T operator()(T v) const{
				return f(v);
			}
		private:
			F f;
	};

	class Sigmoid : public VectorizableFunction<double>{
		public:
			double operator()(double z) const;
	};

	class SigmoidPrime : public VectorizableFunction<double>{
		public:
			double operator()(double z) const;
	};

	class ReLU : public VectorizableFunction<double>{
		public:
			double operator()(double z) const;
	};

	class ReLUPrime : public VectorizableFunction<double>{
		public:
			double operator()(double z) const;
	};

	class LeakyReLU : public VectorizableFunction<double>{
		public:
			LeakyReLU(double slopeWhenNegative);
			double operator()(double z) const;
		private:
			double m_slopeWhenNegative;
	};

	class LeakyReLUPrime : public VectorizableFunction<double>{
		public:
			LeakyReLUPrime(double slopeWhenNegative);
			double operator()(double z) const;
		private:
			double m_slopeWhenNegative;
	};

	class Tanh : public VectorizableFunction<double>{
		public:
			double operator()(double z) const;
	};

	class TanhPrime : public VectorizableFunction<double>{
		public:
			double operator()(double z) const;
	};

	template<class T>
	class DifferentiableFunction{
		public:
			DifferentiableFunction(VectorizableFunction<T> & forward, VectorizableFunction<T> & derivative):
				forward(forward),
				derivative(derivative)
			{
			}

			virtual ~DifferentiableFunction() = default;

			VectorizableFunction<T> & forward;
			VectorizableFunction<T> & derivative;
	};

	template<class T, class ForwardFunction, class DerivativeFunction>
	class DifferentiableFunctionHelper : public DifferentiableFunction<T>{
		public:
			DifferentiableFunctionHelper() :
				DifferentiableFunction(m_forward, m_derivative)
			{
			}

			virtual ~DifferentiableFunctionHelper() = default;

		private:
			ForwardFunction m_forward;
			DerivativeFunction m_derivative;
	};

	class SigmoidActivation : public DifferentiableFunctionHelper<double, Sigmoid, SigmoidPrime>{};
	class ReLUActivation : public DifferentiableFunctionHelper<double, ReLU, ReLUPrime>{};
	class TanhActivation : public DifferentiableFunctionHelper<double, Tanh, TanhPrime>{};

	class IdentityActivation : public DifferentiableFunctionHelper<
		double,
		VectorizableFunctionHelper<double, Jargon::Functional::NullTransform<double> >,
		VectorizableFunctionHelper<double, Jargon::Functional::Unity<double> >
	>{};

	class LeakyReLUActivation : public DifferentiableFunction<double>{
		public:
			LeakyReLUActivation(double slopeWhenNegative);

		private:
			LeakyReLU m_forward;
			LeakyReLUPrime m_derivative;
	};

	class CostFunction{
		public:
			virtual ~CostFunction() = default;
			virtual Eigen::VectorXd computeCostGradient(const Eigen::VectorXd & result, const Eigen::VectorXd & expected) const = 0;
	};

	class QuadraticCost : public CostFunction{
		public:
			virtual Eigen::VectorXd computeCostGradient(const Eigen::VectorXd & result, const Eigen::VectorXd & expected) const{
				return result - expected;
			}
	};

	Eigen::VectorXd softmax(const Eigen::VectorXd & v);
}
}

#endif
