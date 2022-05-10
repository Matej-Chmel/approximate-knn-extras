#pragma once
#include <random>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace chm {
	using uint = unsigned int;

	struct Node {
		float distance;
		uint id;

		Node();
		Node(const float distance, const uint id);
		bool operator==(const Node& o) const;
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

	template<typename T> std::vector<T> generateData(const size_t count, const T minVal, const T maxVal, const uint seed);
	std::vector<Node> generateNodes(const uint count, const float minDist, const float maxDist, const uint seed);

	template<typename T>
	using UniformDistribution = typename std::conditional<
		std::is_floating_point<T>::value, std::uniform_real_distribution<T>, typename std::conditional<
		std::is_integral<T>::value, std::uniform_int_distribution<T>, void>::type
	>::type;

	template<typename T>
	std::vector<T> generateData(const size_t count, const T minVal, const T maxVal, const uint seed) {
		if(minVal > maxVal) {
			std::stringstream s;
			s << "Minimum (" << minVal << ") can't be greater than maximum (" << maxVal << ").";
			throw std::runtime_error(s.str());
		}

		UniformDistribution<T> dist(minVal, maxVal);
		std::default_random_engine gen(seed);
		std::vector<T> res;
		res.reserve(count);

		for(size_t i = 0; i < count; i++)
			res.emplace_back(dist(gen));

		return res;
	}
}
