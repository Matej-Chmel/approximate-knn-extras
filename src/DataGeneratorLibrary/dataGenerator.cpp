#include "dataGenerator.hpp"

namespace chm {
	Node::Node() : distance(0.f), id(0) {}
	Node::Node(const float distance, const uint id) : distance(distance), id(id) {}

	bool Node::operator==(const Node& o) const {
		return this->distance == o.distance;
	}

	std::vector<Node> generateNodes(const uint count, const float minDist, const float maxDist, const uint seed) {
		if(minDist > maxDist) {
			std::stringstream s;
			s << "Minimum distance (" << minDist << ") can't be greater than maximum distance (" << maxDist << ").";
			throw std::runtime_error(s.str());
		}

		std::uniform_real_distribution<float> dist(minDist, maxDist);
		std::default_random_engine gen(seed);
		std::vector<Node> res;
		res.reserve(count);

		for(uint i = 0; i < count; i++)
			res.emplace_back(dist(gen), i);

		return res;
	}
}
