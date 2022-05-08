#include <random>
#include <sstream>
#include "heap.hpp"

namespace chm {
	Node::Node(const float distance, const unsigned int id) : distance(distance), id(id) {}

	bool Node::operator!=(const Node& o) const {
		return this->distance != o.distance;
	}

	void HeapResult::add(const Node& n) {
		this->nodes.emplace_back(n.distance, n.id);
	}

	bool HeapResult::operator!=(const HeapResult& o) const {
		if(this->nodes.size() != o.nodes.size())
			return true;

		const auto len = this->nodes.size();

		for(size_t i = 0; i < len; i++)
			if(this->nodes[i] != o.nodes[i])
				return true;
		return false;
	}

	HeapResult::HeapResult(const size_t maxLen) {
		this->nodes.reserve(maxLen);
	}

	HeapResult::HeapResult(const std::vector<Node>& nodes) : nodes(nodes) {
		std::sort(this->nodes.begin(), this->nodes.end(), SortComparator());
	}

	HeapResult HeapArgs::getCorrectRes() const {
		return HeapResult(this->nodes);
	}

	size_t HeapArgs::getNodeCount() const {
		return this->nodes.size();
	}

	HeapArgs::HeapArgs(const std::vector<Node>& nodes) : nodes(nodes) {}

	std::vector<Node> generateNodes(const unsigned int count, const float minDist, const float maxDist, const unsigned int seed) {
		if(minDist > maxDist) {
			std::stringstream s;
			s << "Minimum distance (" << minDist << ") can't be greater than maximum distance (" << maxDist << ").";
			throw std::runtime_error(s.str());
		}

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
			res.add(queue.top());
			queue.pop();
		}

		return res;
	}
}
