#pragma once
#include <algorithm>
#include <queue>
#include "BenchmarkLibrary/Result.hpp"
#include "DataGeneratorLibrary/dataGenerator.hpp"

namespace chm {
	class HeapResult : public VectorResult<HeapResult, Node> {
	public:
		void emplace(const Node& n);
		HeapResult() = default;
		HeapResult(const size_t maxLen);
		HeapResult(const std::vector<Node>& nodes);
	};

	struct HeapArgs : public NoSetupArgs, public VectorArgs<Node, HeapResult> {
		HeapArgs(const std::vector<Node>& nodes);
	};

	HeapArgs::Result::Opt runPriorityQueue(const HeapArgs& args);
	template<bool reserveMemory> HeapArgs::Result::Opt runPushPopHeap(const HeapArgs& args);

	template<bool reserveMemory>
	inline HeapArgs::Result::Opt runPushPopHeap(const HeapArgs& args) {
		std::vector<Node> heap;
		const auto nodeCount = args.getItemCount();
		const auto& nodes = args.getVectorRef();
		auto res = std::make_optional<HeapArgs::Result>(nodeCount);

		if constexpr(reserveMemory)
			heap.reserve(nodeCount);

		for(const auto& n : nodes) {
			heap.emplace_back(n.distance, n.id);
			std::push_heap(heap.begin(), heap.end(), NearHeapComparator());
		}

		while(!heap.empty()) {
			res->emplace(heap.front());
			std::pop_heap(heap.begin(), heap.end(), NearHeapComparator());
			heap.pop_back();
		}

		return res;
	}
}
