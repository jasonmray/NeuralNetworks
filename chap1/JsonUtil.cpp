#include "JsonUtil.h"

#include "Jargon/Functional.h"


namespace J{
namespace JsonUtil{

	Dropbox::Json::array jsonifyMatrixCols(const Eigen::MatrixXd & src){
		ConstEigenColsView<Eigen::MatrixXd> colsView(src);
		Dropbox::Json::array result;
		Jargon::Functional::map(colsView, result, EigenValueGetter());
		return result;
	}

	Dropbox::Json::array jsonify(const Eigen::MatrixXd & src){
		ConstEigenRowsView<Eigen::MatrixXd> rowsView(src);
		Dropbox::Json::array result;
		Jargon::Functional::map(rowsView, result, jsonifyMatrixCols);
		return result;
	}

	Dropbox::Json::array jsonify(const Eigen::VectorXd & src){
		ConstEigenRowsView<Eigen::VectorXd> rowsView(src);
		Dropbox::Json::array result;
		Jargon::Functional::map(rowsView, result, EigenValueGetter());
		return result;
	}

	Dropbox::Json::array jsonify(const std::vector<size_t>& v){
		Dropbox::Json::array result;
		for( auto & val : v ){
			result.push_back(Dropbox::Json((int)val));
		}
		return result;
	}

	Dropbox::Json::object jsonify(const NetworkLayer & layer){
		return layer.serialize();
	}

	Eigen::VectorXd parseVector(const Dropbox::Json & val){
		auto & valueArray(val.array_items());
		Eigen::VectorXd result;
		result.resize(valueArray.size());

		Eigen::VectorXd::Index i = 0;
		for( auto & v : valueArray ){
			result[i] = v.number_value();
			++i;
		}

		return result;
	}

	Eigen::MatrixXd parseMatrix(const Dropbox::Json & val){
		auto & valueArray(val.array_items());
		Eigen::MatrixXd result;
		result.resize(valueArray.size(), valueArray[0].array_items().size());

		Eigen::MatrixXd::Index i = 0;
		for( auto & v : valueArray ){
			result.row(i) = parseVector(v.array_items());
			++i;
		}

		return result;
	}

}
}
