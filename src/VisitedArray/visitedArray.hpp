#pragma once
#include "BenchmarkLibrary/Args.hpp"
#include "DataGeneratorLibrary/dataGenerator.hpp"

namespace chm {
	struct VisitedArrayTask {
		std::vector<uint> items;
		std::vector<uint> queries;

		VisitedArrayTask(const uint itemCount, const uint queryCount, const uint seed, const bool equalSeed = false);
	};

	class VisitedArrayResult : public VectorResult<VisitedArrayResult, uint> {
	public:
		VisitedArrayResult() = default;
		VisitedArrayResult(const size_t queryCount);
		VisitedArrayResult(const std::vector<uint>& items, const std::vector<uint>& queries);
	};

	struct VisitedArrayArgs : public VectorNoSetupArgs<uint, VisitedArrayResult> {
		const std::vector<uint> queries;

		Result::Opt getCorrectRes() const;
		size_t getQueryCount() const;
		VisitedArrayArgs(const VisitedArrayTask& t);
	};

	template<typename T> VisitedArrayArgs::Result::Opt runVisitedArray(const VisitedArrayArgs& args);
	void throwZeroItems(const std::string& items);

	template<typename T>
	inline VisitedArrayArgs::Result::Opt runVisitedArray(const VisitedArrayArgs& args) {
		const auto& items = args.getVectorRef();
		auto res = std::make_optional<VisitedArrayArgs::Result>(args.getQueryCount());
		std::vector<T> visited;
		visited.resize(args.getItemCount(), T(0));

		for(const auto& e : items)
			visited[e] = T(1);
		for(const auto& q : args.queries)
			res->add(uint(visited[q]));

		return res;
	}
}
