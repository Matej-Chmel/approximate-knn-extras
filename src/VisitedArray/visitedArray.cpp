#include <algorithm>
#include <stdexcept>
#include "visitedArray.hpp"

namespace chm {
	VisitedArrayTask::VisitedArrayTask(const uint elementCount, const uint queryCount, const uint seed, const bool equalSeed) {
		if(!elementCount)
			throw std::runtime_error("There can't be zero elements.");
		if(!queryCount)
			throw std::runtime_error("There can't be zero queries.");

		const auto maxElem = elementCount - 1;
		this->elements = generateData<uint>(size_t(elementCount), 0, maxElem, seed);
		this->queries = generateData<uint>(size_t(queryCount), 0, maxElem, equalSeed ? seed : seed + 1);
	}

	VisitedArrayResult::VisitedArrayResult(const size_t queryCount) : VectorResult<uint>(queryCount) {}

	VisitedArrayResult::VisitedArrayResult(const std::vector<uint>& elements, const std::vector<uint>& queries)
		: VectorResult<uint>(queries.size()) {

		std::vector<uint> elementsCopy(elements);
		std::sort(elementsCopy.begin(), elementsCopy.end());

		for(const auto& q : queries)
			this->data.emplace_back((uint)(std::binary_search(elementsCopy.begin(), elementsCopy.end(), q)));
	}

	VisitedArrayArgs::Result VisitedArrayArgs::getCorrectRes() const {
		return VisitedArrayArgs::Result(this->elements, this->queries);
	}

	size_t VisitedArrayArgs::getElementCount() const {
		return this->elements.size();
	}

	size_t VisitedArrayArgs::getQueryCount() const {
		return this->queries.size();
	}

	VisitedArrayArgs::VisitedArrayArgs(const VisitedArrayTask& t) : elements(t.elements), queries(t.queries) {}
}
