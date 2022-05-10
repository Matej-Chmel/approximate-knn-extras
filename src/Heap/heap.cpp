#include "heap.hpp"

namespace chm {
	void HeapResult::emplace(const Node& n) {
		this->items.emplace_back(n.distance, n.id);
	}

	HeapResult::HeapResult(const size_t maxLen) : VectorResult<HeapResult, Node>(maxLen) {}

	HeapResult::HeapResult(const std::vector<Node>& nodes) : VectorResult<HeapResult, Node>(nodes) {
		std::sort(this->items.begin(), this->items.end(), SortComparator());
	}

	HeapArgs::HeapArgs(const std::vector<Node>& nodes) : VectorNoSetupArgs<Node, HeapResult>(nodes) {}

	HeapArgs::Result::Opt runPriorityQueue(const HeapArgs& args) {
		const auto& nodes = args.getVectorRef();
		std::priority_queue<Node, std::vector<Node>, NearHeapComparator> queue;
		auto res = std::make_optional<HeapArgs::Result>(args.getItemCount());

		for(const auto& n : nodes)
			queue.emplace(n.distance, n.id);

		while(!queue.empty()) {
			res->emplace(queue.top());
			queue.pop();
		}

		return res;
	}
}
