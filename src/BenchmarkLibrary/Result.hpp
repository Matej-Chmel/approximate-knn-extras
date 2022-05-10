#pragma once
#include <optional>
#include <vector>

namespace chm {
	template<class Derived>
	struct OptResult {
		using Opt = std::optional<Derived>;
	};

	struct UncomparableResult : public OptResult<UncomparableResult> {
		bool operator!=(const UncomparableResult& o) const;
	};

	struct NoSetupArgs {
		using SetupTask = std::nullptr_t;

		void setup(const SetupTask&);
	};

	struct UncomparableArgs : public NoSetupArgs {
		using Result = UncomparableResult;
	};

	template<class Derived, typename T>
	class VectorResult : public OptResult<Derived> {
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

	template<class Derived, typename T>
	inline void VectorResult<Derived, T>::add(const T& t) {
		this->data.push_back(t);
	}

	template<class Derived, typename T>
	inline T* VectorResult<Derived, T>::getData() {
		return this->data.data();
	}

	template<class Derived, typename T>
	inline bool VectorResult<Derived, T>::operator!=(const VectorResult& o) const {
		const auto len = this->data.size();

		if(len != o.data.size())
			return true;

		for(size_t i = 0; i < len; i++)
			if(this->data[i] != o.data[i])
				return true;
		return false;
	}

	template<class Derived, typename T>
	inline VectorResult<Derived, T>::VectorResult(const size_t maxCount, const bool reserveMemory) {
		if(reserveMemory)
			this->data.reserve(maxCount);
		else
			this->data.resize(maxCount);
	}

	template<class Derived, typename T>
	inline VectorResult<Derived, T>::VectorResult(const std::vector<T>& data) : data(data) {}
}
