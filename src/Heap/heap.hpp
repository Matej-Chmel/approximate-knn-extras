#pragma once
#include <algorithm>
#include <queue>
#include "BenchmarkLibrary/Result.hpp"
#include "DataGeneratorLibrary/dataGenerator.hpp"

namespace chm {
	class HeapResult : public VectorResult<Node> {
	public:
		void emplace(const Node& n);
		HeapResult() = default;
		HeapResult(const size_t maxLen);
		HeapResult(const std::vector<Node>& nodes);
	};

	struct HeapArgs {
		using Result = HeapResult;

		const std::vector<Node> nodes;

		Result getCorrectRes() const;
		size_t getNodeCount() const;
		HeapArgs(const std::vector<Node>& nodes);
	};

	HeapArgs::Result runPriorityQueue(const HeapArgs& args);
	template<bool reserveMemory> HeapArgs::Result runPushPopHeap(const HeapArgs& args);

	template<bool reserveMemory>
	inline HeapArgs::Result runPushPopHeap(const HeapArgs& args) {
		std::vector<Node> heap;
		const auto nodeCount = args.getNodeCount();
		HeapArgs::Result res(nodeCount);

		if constexpr(reserveMemory)
			heap.reserve(nodeCount);

		for(const auto& n : args.nodes) {
			heap.emplace_back(n.distance, n.id);
			std::push_heap(heap.begin(), heap.end(), NearHeapComparator());
		}

		while(!heap.empty()) {
			res.emplace(heap.front());
			std::pop_heap(heap.begin(), heap.end(), NearHeapComparator());
			heap.pop_back();
		}

		return res;
	}
}
