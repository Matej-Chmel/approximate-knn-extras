#pragma once
#include <vector>

namespace chm {
	using uint = unsigned int;

	struct VisitedArrayTask {
		std::vector<uint> elements;
		std::vector<uint> queries;

		VisitedArrayTask(const uint elementCount, const uint queryCount, const uint seed, const bool equalSeed = false);
	};

	class VisitedArrayResult {
	public:
		void add(const uint i);
		bool operator!=(const VisitedArrayResult& o) const;
		VisitedArrayResult() = default;
		VisitedArrayResult(const size_t queryCount);
		VisitedArrayResult(const std::vector<uint>& elements, const std::vector<uint>& queries);

	private:
		std::vector<uint> answers;
	};

	struct VisitedArrayArgs {
		using Result = VisitedArrayResult;

		const std::vector<uint> elements;
		const std::vector<uint> queries;

		Result getCorrectRes() const;
		size_t getElementCount() const;
		size_t getQueryCount() const;
		VisitedArrayArgs(const VisitedArrayTask& t);
	};

	template<typename T> VisitedArrayArgs::Result runVisitedArray(const VisitedArrayArgs& args);

	template<typename T>
	inline VisitedArrayArgs::Result runVisitedArray(const VisitedArrayArgs& args) {
		std::vector<T> visited;
		visited.resize(args.getElementCount(), T(0));
		VisitedArrayArgs::Result res(args.getQueryCount());

		for(const auto& e : args.elements)
			visited[e] = T(1);
		for(const auto& q : args.queries)
			res.add(uint(visited[q]));

		return res;
	}
}
