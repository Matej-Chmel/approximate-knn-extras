#pragma once
#include <algorithm>
#include <queue>
#include <vector>

namespace chm {
	struct Node {
		float distance;
		unsigned int id;

		Node(const float distance, const unsigned int id);
		bool operator!=(const Node& o) const;
	};

	struct NearHeapComparator {
		constexpr bool operator()(const Node& a, const Node& b) const noexcept {
			return a.distance > b.distance;
		}
	};

	struct SortComparator {
		constexpr bool operator()(const Node& a, const Node& b) const noexcept {
			return a.distance < b.distance;
		}
	};

	struct HeapArgs {
		class Result {
		public:
			void add(const Node& n);
			bool operator!=(const Result& o) const;
			Result(const size_t maxLen);
			Result(const std::vector<Node>& nodes);

		private:
			std::vector<Node> nodes;
		};

		const std::vector<Node> nodes;

		Result getCorrectRes() const;
		size_t getNodeCount() const;
		HeapArgs(const std::vector<Node>& nodes);
	};

	std::vector<Node> generateNodes(const unsigned int count, const float minDist, const float maxDist, const unsigned int seed);
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
			const auto& n = heap.front();
			res.add(n);
			std::pop_heap(heap.begin(), heap.end(), NearHeapComparator());
			heap.pop_back();
		}

		return res;
	}
}
