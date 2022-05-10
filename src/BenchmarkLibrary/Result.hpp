#pragma once
#include <optional>
#include <stdexcept>
#include <string>
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

	template<typename ItemType, typename ResultType>
	class VectorArgs {
	public:
		using Result = ResultType;

		typename Result::Opt getCorrectRes() const;
		const ItemType* const getData() const;
		const size_t getItemCount() const;
		const std::vector<ItemType>& getVectorRef() const;
		VectorArgs() = default;
		VectorArgs(const std::vector<ItemType>& items);

	protected:
		std::vector<ItemType> items;
	};

	template<class Derived, typename ItemType>
	class VectorResult : public OptResult<Derived> {
	public:
		void add(const ItemType& i);
		ItemType* getData();
		bool operator!=(const VectorResult& o) const;
		VectorResult() = default;
		VectorResult(const size_t maxCount, const bool reserveMemory = true);
		VectorResult(const std::vector<ItemType>& items);

	protected:
		std::vector<ItemType> items;
	};

	void throwNotImplemented(const std::string& className, const std::string& methodName);

	template<typename ItemType, typename ResultType>
	inline typename VectorArgs<ItemType, ResultType>::Result::Opt VectorArgs<ItemType, ResultType>::getCorrectRes() const {
		return std::make_optional<ResultType>(this->items);
	}

	template<typename ItemType, typename ResultType>
	inline const ItemType* const VectorArgs<ItemType, ResultType>::getData() const {
		return this->items.data();
	}

	template<typename ItemType, typename ResultType>
	inline const size_t VectorArgs<ItemType, ResultType>::getItemCount() const {
		return this->items.size();
	}

	template<typename ItemType, typename ResultType>
	inline const std::vector<ItemType>& VectorArgs<ItemType, ResultType>::getVectorRef() const {
		return this->items;
	}

	template<typename ItemType, typename ResultType>
	inline VectorArgs<ItemType, ResultType>::VectorArgs(const std::vector<ItemType>& items) : items(items) {}

	template<class Derived, typename ItemType>
	inline void VectorResult<Derived, ItemType>::add(const ItemType& i) {
		this->items.push_back(i);
	}

	template<class Derived, typename ItemType>
	inline ItemType* VectorResult<Derived, ItemType>::getData() {
		return this->items.data();
	}

	template<class Derived, typename ItemType>
	inline bool VectorResult<Derived, ItemType>::operator!=(const VectorResult& o) const {
		const auto len = this->items.size();

		if(len != o.items.size())
			return true;

		for(size_t i = 0; i < len; i++)
			if(this->items[i] != o.items[i])
				return true;
		return false;
	}

	template<class Derived, typename ItemType>
	inline VectorResult<Derived, ItemType>::VectorResult(const size_t maxCount, const bool reserveMemory) {
		if(reserveMemory)
			this->items.reserve(maxCount);
		else
			this->items.resize(maxCount);
	}

	template<class Derived, typename ItemType>
	inline VectorResult<Derived, ItemType>::VectorResult(const std::vector<ItemType>& items) : items(items) {}
}
