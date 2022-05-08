#include <random>
#include "heap.hpp"

namespace chm {
	Node::Node(const float distance, const unsigned int id) : distance(distance), id(id) {}

	bool Node::operator!=(const Node& o) const {
		return this->distance != o.distance;
	}

	void HeapArgs::Result::add(const Node& n) {
		this->nodes.emplace_back(n.distance, n.id);
	}

	bool HeapArgs::Result::operator!=(const Result& o) const {
		if(this->nodes.size() != o.nodes.size())
			return true;

		const auto len = this->nodes.size();

		for(size_t i = 0; i < len; i++)
			if(this->nodes[i] != o.nodes[i])
				return true;
		return false;
	}

	HeapArgs::Result::Result(const size_t maxLen) {
		this->nodes.reserve(maxLen);
	}

	HeapArgs::Result::Result(const std::vector<Node>& nodes) : nodes(nodes) {
		std::sort(this->nodes.begin(), this->nodes.end(), SortComparator());
	}

	HeapArgs::Result HeapArgs::getCorrectRes() const {
		return HeapArgs::Result(this->nodes);
	}

	size_t HeapArgs::getNodeCount() const {
		return this->nodes.size();
	}

	HeapArgs::HeapArgs(const std::vector<Node>& nodes) : nodes(nodes) {}

	std::vector<Node> generateNodes(const unsigned int count, const float minDist, const float maxDist, const unsigned int seed) {
		std::uniform_real_distribution<float> dist(minDist, maxDist);
		std::default_random_engine gen(seed);
		std::vector<Node> res;
		res.reserve(count);

		for(unsigned int i = 0; i < count; i++)
			res.emplace_back(dist(gen), i);

		return res;
	}

	HeapArgs::Result runPriorityQueue(const HeapArgs& args) {
		std::priority_queue<Node, std::vector<Node>, NearHeapComparator> queue;
		HeapArgs::Result res(args.getNodeCount());

		for(const auto& n : args.nodes)
			queue.emplace(n.distance, n.id);

		while(!queue.empty()) {
			const auto& n = queue.top();
			res.add(n);
			queue.pop();
		}

		return res;
	}
}
