#include <algorithm>
#include <random>
#include <sstream>
#include <stdexcept>
#include "DataGeneratorLibrary/dataGenerator.hpp"
#include "visitedArray.hpp"

namespace chm {
	VisitedArrayTask::VisitedArrayTask(const uint elementCount, const uint queryCount, const uint seed, const bool equalSeed) {
		if(!elementCount)
			throw std::runtime_error("There can't be zero elements.");
		if(!queryCount)
			throw std::runtime_error("There can't be zero queries.");

		const auto maxElem = elementCount - 1;
		this->elements = generateData<unsigned int>(size_t(elementCount), 0, maxElem, seed);
		this->queries = generateData<unsigned int>(size_t(queryCount), 0, maxElem, equalSeed ? seed : seed + 1);
	}

	void VisitedArrayResult::add(const uint i) {
		this->answers.emplace_back(i);
	}

	bool VisitedArrayResult::operator!=(const VisitedArrayResult& o) const {
		if(this->answers.size() != o.answers.size())
			return true;

		const auto len = this->answers.size();

		for(size_t i = 0; i < len; i++)
			if(this->answers[i] != o.answers[i])
				return true;
		return false;
	}

	VisitedArrayResult::VisitedArrayResult(const size_t queryCount) {
		this->answers.reserve(queryCount);
	}

	VisitedArrayResult::VisitedArrayResult(const std::vector<uint>& elements, const std::vector<uint>& queries) {
		this->answers.reserve(queries.size());
		std::vector<uint> elementsCopy(elements);
		std::sort(elementsCopy.begin(), elementsCopy.end());

		for(const auto& q : queries)
			this->answers.emplace_back((uint)(std::binary_search(elementsCopy.begin(), elementsCopy.end(), q)));
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
