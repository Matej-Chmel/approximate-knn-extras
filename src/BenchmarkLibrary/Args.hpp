#pragma once
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace chm {
	template<typename ResultType, typename SetupTaskType>
	struct BenchmarkArgs {
		using Result = ResultType;
		using SetupTask = SetupTaskType;

		void setup(const SetupTask&);
	};

	template<class Derived>
	struct OptResult {
		using Opt = std::optional<Derived>;
	};

	struct UncomparableResult : public OptResult<UncomparableResult> {
		bool operator!=(const UncomparableResult& o) const;
	};

	struct UncomparableArgs : public BenchmarkArgs<UncomparableResult, std::nullptr_t> {};

	template<typename ItemType, typename ResultType, typename SetupTaskType>
	class VectorArgs : public BenchmarkArgs<ResultType, SetupTaskType> {
	public:
		typename VectorArgs<ItemType, ResultType, SetupTaskType>::Result::Opt getCorrectRes() const;
		const ItemType* const getData() const;
		const size_t getItemCount() const;
		const std::vector<ItemType>& getVectorRef() const;
		VectorArgs() = default;
		VectorArgs(const std::vector<ItemType>& items);

	protected:
		std::vector<ItemType> items;
	};

	template<typename ItemType, typename ResultType>
	struct VectorNoSetupArgs : public VectorArgs<ItemType, ResultType, std::nullptr_t> {
		VectorNoSetupArgs() = default;
		VectorNoSetupArgs(const std::vector<ItemType>& items);
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

	template<typename ResultType, typename SetupTaskType>
	void BenchmarkArgs<ResultType, SetupTaskType>::setup(const SetupTask&) {
		throwNotImplemented("BenchmarkArgs", "setup");
	}

	template<typename ItemType, typename ResultType, typename SetupTaskType>
	inline typename VectorArgs<ItemType, ResultType, SetupTaskType>::Result::Opt VectorArgs<
		ItemType, ResultType, SetupTaskType>::getCorrectRes() const {

		return std::make_optional<ResultType>(this->items);
	}

	template<typename ItemType, typename ResultType, typename SetupTaskType>
	inline const ItemType* const VectorArgs<ItemType, ResultType, SetupTaskType>::getData() const {
		return this->items.data();
	}

	template<typename ItemType, typename ResultType, typename SetupTaskType>
	inline const size_t VectorArgs<ItemType, ResultType, SetupTaskType>::getItemCount() const {
		return this->items.size();
	}

	template<typename ItemType, typename ResultType, typename SetupTaskType>
	inline const std::vector<ItemType>& VectorArgs<ItemType, ResultType, SetupTaskType>::getVectorRef() const {
		return this->items;
	}

	template<typename ItemType, typename ResultType, typename SetupTaskType>
	inline VectorArgs<ItemType, ResultType, SetupTaskType>::VectorArgs(const std::vector<ItemType>& items) : items(items) {}

	template<typename ItemType, typename ResultType>
	inline VectorNoSetupArgs<ItemType, ResultType>::VectorNoSetupArgs(const std::vector<ItemType>& items)
		: VectorArgs<ItemType, ResultType, std::nullptr_t>(items) {}

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
		return this->items != o.items;
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
