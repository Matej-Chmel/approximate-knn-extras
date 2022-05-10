#include "heap.hpp"

namespace chm {
	void HeapResult::emplace(const Node& n) {
		this->data.emplace_back(n.distance, n.id);
	}

	HeapResult::HeapResult(const size_t maxLen) : VectorResult<HeapResult, Node>(maxLen) {}

	HeapResult::HeapResult(const std::vector<Node>& nodes) : VectorResult<HeapResult, Node>(nodes) {
		std::sort(this->data.begin(), this->data.end(), SortComparator());
	}

	HeapArgs::Result::Opt HeapArgs::getCorrectRes() const {
		return std::make_optional<HeapArgs::Result>(this->nodes);
	}

	size_t HeapArgs::getNodeCount() const {
		return this->nodes.size();
	}

	HeapArgs::HeapArgs(const std::vector<Node>& nodes) : nodes(nodes) {}

	HeapArgs::Result::Opt runPriorityQueue(const HeapArgs& args) {
		std::priority_queue<Node, std::vector<Node>, NearHeapComparator> queue;
		auto res = std::make_optional<HeapArgs::Result>(args.getNodeCount());

		for(const auto& n : args.nodes)
			queue.emplace(n.distance, n.id);

		while(!queue.empty()) {
			res->emplace(queue.top());
			queue.pop();
		}

		return res;
	}
}
