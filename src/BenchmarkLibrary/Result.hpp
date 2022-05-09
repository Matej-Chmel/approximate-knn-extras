#pragma once
#include <vector>

namespace chm {
	struct UncomparableResult {
		bool operator!=(const UncomparableResult& o) const;
	};

	template<typename T>
	class VectorResult {
	public:
		void add(const T& t);
		T* getData();
		bool operator!=(const VectorResult& o) const;
		VectorResult() = default;
		VectorResult(const size_t maxCount, const bool reserveMemory = true);
		VectorResult(const std::vector<T>& data);

	protected:
		std::vector<T> data;
	};

	template<typename T>
	inline void VectorResult<T>::add(const T& t) {
		this->data.push_back(t);
	}

	template<typename T>
	inline T* VectorResult<T>::getData() {
		return this->data.data();
	}

	template<typename T>
	inline bool VectorResult<T>::operator!=(const VectorResult& o) const {
		const auto len = this->data.size();

		if(len != o.data.size())
			return true;

		for(size_t i = 0; i < len; i++)
			if(this->data[i] != o.data[i])
				return true;
		return false;
	}

	template<typename T>
	inline VectorResult<T>::VectorResult(const size_t maxCount, const bool reserveMemory) {
		if(reserveMemory)
			this->data.reserve(maxCount);
		else
			this->data.resize(maxCount);
	}

	template<typename T>
	inline VectorResult<T>::VectorResult(const std::vector<T>& data) : data(data) {}
}
