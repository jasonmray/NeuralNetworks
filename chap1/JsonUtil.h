
#ifndef J_JSONUTIL_H
#define J_JSONUTIL_H

#include "EigenUtil.h"
#include "NetworkLayer.h"

#include "Eigen/Eigen"
#include "DropboxJson.h"


namespace J{
namespace JsonUtil{

	Dropbox::Json::array jsonifyMatrixCols(const Eigen::MatrixXd & src);
	Dropbox::Json::array jsonify(const Eigen::MatrixXd & src);
	Dropbox::Json::array jsonify(const Eigen::VectorXd & src);
	Dropbox::Json::array jsonify(const std::vector<size_t> & v);
	Dropbox::Json::object jsonify(const NetworkLayer & layer);

	template<class T>
	Dropbox::Json::array jsonify(T begin, T end){
		Dropbox::Json::array result;
		for(T it = begin; it != end; ++it ){
			result.push_back(jsonify(*it));
		}
		return result;
	}

	Eigen::VectorXd parseVector(const Dropbox::Json & val);
	Eigen::MatrixXd parseMatrix(const Dropbox::Json & val);

}
}

#endif
