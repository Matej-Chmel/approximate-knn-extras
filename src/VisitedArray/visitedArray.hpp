#pragma once
#include "BenchmarkLibrary/Result.hpp"
#include "DataGeneratorLibrary/dataGenerator.hpp"

namespace chm {
	struct VisitedArrayTask {
		std::vector<uint> elements;
		std::vector<uint> queries;

		VisitedArrayTask(const uint elementCount, const uint queryCount, const uint seed, const bool equalSeed = false);
	};

	class VisitedArrayResult : public VectorResult<VisitedArrayResult, uint> {
	public:
		VisitedArrayResult() = default;
		VisitedArrayResult(const size_t queryCount);
		VisitedArrayResult(const std::vector<uint>& elements, const std::vector<uint>& queries);
	};

	struct VisitedArrayArgs : public NoSetupArgs {
		using Result = VisitedArrayResult;

		const std::vector<uint> elements;
		const std::vector<uint> queries;

		Result::Opt getCorrectRes() const;
		size_t getElementCount() const;
		size_t getQueryCount() const;
		VisitedArrayArgs(const VisitedArrayTask& t);
	};

	template<typename T> VisitedArrayArgs::Result::Opt runVisitedArray(const VisitedArrayArgs& args);

	template<typename T>
	inline VisitedArrayArgs::Result::Opt runVisitedArray(const VisitedArrayArgs& args) {
		std::vector<T> visited;
		visited.resize(args.getElementCount(), T(0));
		auto res = std::make_optional<VisitedArrayArgs::Result>(args.getQueryCount());

		for(const auto& e : args.elements)
			visited[e] = T(1);
		for(const auto& q : args.queries)
			res->add(uint(visited[q]));

		return res;
	}
}
