#include "heap.hpp"

namespace chm {
	void HeapResult::emplace(const Node& n) {
		this->data.emplace_back(n.distance, n.id);
	}

	HeapResult::HeapResult(const size_t maxLen) : VectorResult<Node>(maxLen) {}

	HeapResult::HeapResult(const std::vector<Node>& nodes) : VectorResult<Node>(nodes) {
		std::sort(this->data.begin(), this->data.end(), SortComparator());
	}

	HeapResult HeapArgs::getCorrectRes() const {
		return HeapResult(this->nodes);
	}

	size_t HeapArgs::getNodeCount() const {
		return this->nodes.size();
	}

	HeapArgs::HeapArgs(const std::vector<Node>& nodes) : nodes(nodes) {}

	HeapArgs::Result runPriorityQueue(const HeapArgs& args) {
		std::priority_queue<Node, std::vector<Node>, NearHeapComparator> queue;
		HeapArgs::Result res(args.getNodeCount());

		for(const auto& n : args.nodes)
			queue.emplace(n.distance, n.id);

		while(!queue.empty()) {
			res.emplace(queue.top());
			queue.pop();
		}

		return res;
	}
}
