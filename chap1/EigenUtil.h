
#ifndef J_EIGENUTIL_H
#define J_EIGENUTIL_H

#include "Eigen/Eigen"

#include <iterator>
#include <functional>


namespace J{

	template<class EigenT>
	class ConstEigenRowIterator : public std::iterator<std::forward_iterator_tag, typename EigenT::ConstRowXpr>{
		public:
			typedef ConstEigenRowIterator<EigenT> ThisType;

			ConstEigenRowIterator():
				m_iteratee(NULL),
				m_index()
			{
			}

			ConstEigenRowIterator(const EigenT & iteratee):
				m_iteratee(&iteratee),
				m_index()
			{
			}

			ConstEigenRowIterator(const EigenT & iteratee, typename EigenT::Index index):
				m_iteratee(&iteratee),
				m_index(index)
			{
			}

			ThisType & operator=(const ThisType& other){
				m_iteratee = other.m_iteratee;
				m_index = other.m_index;
				return *this;
			}

			bool operator==(const ThisType& other) const{
				return m_iteratee == other.m_iteratee && m_index == other.m_index;
			}

			bool operator!=(const ThisType& other) const{
				return ! this->operator==(other);
			}

			ThisType & operator++(){
				++m_index;
				return *this;
			}

			ThisType operator++(int count){
				ThisType result(m_index);
				m_index += count;
				return result;
			}

			typename EigenT::ConstRowXpr operator*() const{
				return (*m_iteratee).row(m_index);
			}

			typename EigenT::ConstRowXpr operator->() const{
				return (*m_iteratee).row(m_index);
			}

		private:
			const EigenT * m_iteratee;
			typename EigenT::Index m_index;

	};

	template<class EigenT>
	class EigenRowIterator : public std::iterator<std::forward_iterator_tag, typename EigenT::RowXpr>{
		public:
			typedef EigenRowIterator<EigenT> ThisType;

			EigenRowIterator():
				m_iteratee(NULL),
				m_index()
			{
			}

			EigenRowIterator(EigenT & iteratee):
				m_iteratee(&iteratee),
				m_index()
			{
			}

			EigenRowIterator(EigenT & iteratee, typename EigenT::Index index):
				m_iteratee(&iteratee),
				m_index(index)
			{
			}

			ThisType & operator=(const ThisType& other){
				m_iteratee = other.m_iteratee;
				m_index = other.m_index;
				return *this;
			}

			bool operator==(const ThisType& other) const{
				return m_iteratee == other.m_iteratee && m_index == other.m_index;
			}

			bool operator!=(const ThisType& other) const{
				return ! this->operator==(other);
			}

			ThisType & operator++(){
				++m_index;
				return *this;
			}

			ThisType operator++(int count){
				ThisType result(m_index);
				m_index += count;
				return result;
			}

			typename EigenT::RowXpr operator*(){
				return (*m_iteratee).row(m_index);
			}

			typename EigenT::ConstRowXpr operator*() const{
				return (*m_iteratee).row(m_index);
			}

			typename EigenT::RowXpr operator->() {
				return (*m_iteratee).row(m_index);
			}

			typename EigenT::ConstRowXpr operator->() const{
				return (*m_iteratee).row(m_index);
			}
		private:
			EigenT * m_iteratee;
			typename EigenT::Index m_index;
	};




	template<class EigenT>
	class ConstEigenColIterator : public std::iterator<std::forward_iterator_tag, typename EigenT::ConstColXpr>{
		public:
			typedef ConstEigenColIterator<EigenT> ThisType;

			ConstEigenColIterator():
				m_iteratee(NULL),
				m_index()
			{
			}

			ConstEigenColIterator(const EigenT & iteratee):
				m_iteratee(&iteratee),
				m_index()
			{
			}

			ConstEigenColIterator(const EigenT & iteratee, typename EigenT::Index index):
				m_iteratee(&iteratee),
				m_index(index)
			{
			}

			ThisType & operator=(const ThisType& other){
				m_iteratee = other.m_iteratee;
				m_index = other.m_index;
				return *this;
			}

			bool operator==(const ThisType& other) const{
				return m_iteratee == other.m_iteratee && m_index == other.m_index;
			}

			bool operator!=(const ThisType& other) const{
				return ! this->operator==(other);
			}

			ThisType & operator++(){
				++m_index;
				return *this;
			}

			ThisType operator++(int count){
				ThisType result(m_index);
				m_index += count;
				return result;
			}

			typename EigenT::ConstColXpr operator*() const{
				return (*m_iteratee).col(m_index);
			}

			typename EigenT::ConstColXpr operator->() const{
				return (*m_iteratee).col(m_index);
			}

		private:
			const EigenT * m_iteratee;
			typename EigenT::Index m_index;

	};

	template<class EigenT>
	class EigenColIterator : public std::iterator<std::forward_iterator_tag, typename EigenT::ColXpr>{
		public:
			typedef EigenColIterator<EigenT> ThisType;

			EigenColIterator():
				m_iteratee(NULL),
				m_index()
			{
			}

			EigenColIterator(EigenT & iteratee):
				m_iteratee(&iteratee),
				m_index()
			{
			}

			EigenColIterator(EigenT & iteratee, typename EigenT::Index index):
				m_iteratee(&iteratee),
				m_index(index)
			{
			}

			ThisType & operator=(const ThisType& other){
				m_iteratee = other.m_iteratee;
				m_index = other.m_index;
				return *this;
			}

			bool operator==(const ThisType& other) const{
				return m_iteratee == other.m_iteratee && m_index == other.m_index;
			}

			bool operator!=(const ThisType& other) const{
				return ! this->operator==(other);
			}

			ThisType & operator++(){
				++m_index;
				return *this;
			}

			ThisType operator++(int count){
				ThisType result(m_index);
				m_index += count;
				return result;
			}

			typename EigenT::ColXpr & operator*(){
				return (*m_iteratee).col(m_index);
			}

			typename EigenT::ConstColXpr & operator*() const{
				return (*m_iteratee).col(m_index);
			}

			typename EigenT::ColXpr & operator->(){
				return (*m_iteratee).col(m_index);
			}

			typename EigenT::ConstColXpr & operator->() const{
				return (*m_iteratee).col(m_index);
			}

		private:
			EigenT * m_iteratee;
			typename EigenT::Index m_index;
	};

	template<class EigenT>
	class EigenRowsView{
		public:
			typedef EigenRowsView<EigenT> ThisType;

			EigenRowsView(EigenT & iteratee):
				m_iteratee(iteratee)
			{
			}

			EigenRowIterator<EigenT> begin() const {
				return EigenRowIterator<EigenT>(m_iteratee);
			}

			EigenRowIterator<EigenT> end() const {
				return EigenRowIterator<EigenT>(m_iteratee, m_iteratee.rows());
			}

			ConstEigenRowIterator<EigenT> cbegin() const {
				return ConstEigenRowIterator<EigenT>(m_iteratee);
			}

			ConstEigenRowIterator<EigenT> cend() const {
				return ConstEigenRowIterator<EigenT>(m_iteratee, m_iteratee.rows());
			}

			size_t size() const{
				return m_iteratee.rows();
			}

		private:
			EigenT & m_iteratee;
	};

	template<class EigenT>
	class ConstEigenRowsView{
		public:
			typedef ConstEigenRowsView<EigenT> ThisType;

			ConstEigenRowsView(const EigenT & iteratee):
				m_iteratee(iteratee)
			{
			}

			ConstEigenRowIterator<EigenT> begin() const {
				return ConstEigenRowIterator<EigenT>(m_iteratee);
			}

			ConstEigenRowIterator<EigenT> end() const {
				return ConstEigenRowIterator<EigenT>(m_iteratee, m_iteratee.rows());
			}

			size_t size() const{
				return m_iteratee.rows();
			}

		private:
			const EigenT & m_iteratee;
	};

	template<class EigenT>
	class EigenColsView{
		public:
			typedef EigenColsView<EigenT> ThisType;

			EigenColsView(EigenT & iteratee):
				m_iteratee(iteratee)
			{
			}

			EigenColIterator<EigenT> begin() const {
				return EigenColIterator<EigenT>(m_iteratee);
			}

			EigenColIterator<EigenT> end() const {
				return EigenColIterator<EigenT>(m_iteratee, m_iteratee.cols());
			}

			ConstEigenColIterator<EigenT> cbegin() const {
				return ConstEigenColIterator<EigenT>(m_iteratee);
			}

			ConstEigenColIterator<EigenT> cend() const {
				return ConstEigenColIterator<EigenT>(m_iteratee, m_iteratee.cols());
			}

			size_t size() const{
				return m_iteratee.cols();
			}

		private:
			EigenT & m_iteratee;
	};

	template<class EigenT>
	class ConstEigenColsView{
		public:
			typedef ConstEigenColsView<EigenT> ThisType;

			ConstEigenColsView(const EigenT & iteratee):
				m_iteratee(iteratee)
			{
			}

			ConstEigenColIterator<EigenT> begin() const {
				return ConstEigenColIterator<EigenT>(m_iteratee);
			}

			ConstEigenColIterator<EigenT> end() const {
				return ConstEigenColIterator<EigenT>(m_iteratee, m_iteratee.cols());
			}

			size_t size() const{
				return m_iteratee.cols();
			}

		private:
			const EigenT & m_iteratee;
	};


	template<class EigenT>
	class EigenIterationHelper{
		public:
			typedef EigenIterationHelper<EigenT> ThisType;

			EigenIterationHelper(EigenT & iteratee):
				m_iteratee(iteratee)
			{
			}


			EigenRowsView<EigenT> rows() const {
				return EigenRowsView<EigenT>(m_iteratee);
			}

			EigenColsView<EigenT> cols() const {
				return EigenColsView<EigenT>(m_iteratee);
			}

		private:
			EigenT & m_iteratee;
	};

	template<class EigenT>
	class ConstEigenIterationHelper{
		public:
			typedef ConstEigenIterationHelper<EigenT> ThisType;

			ConstEigenIterationHelper(const EigenT & iteratee):
				m_iteratee(iteratee)
			{
			}


			ConstEigenRowsView<EigenT> rows() const {
				return ConstEigenRowsView<EigenT>(m_iteratee);
			}

			ConstEigenColsView<EigenT> cols() const{
				return ConstEigenColsView<EigenT>(m_iteratee);
			}

		private:
			const EigenT & m_iteratee;
	};

	class EigenValueGetter : public std::unary_function<Eigen::VectorXd, double>{
		public:
			double operator()(const Eigen::VectorXd & v){
				return v.value();
			}
	};

}

#endif
