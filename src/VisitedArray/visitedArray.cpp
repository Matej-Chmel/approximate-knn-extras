#include <algorithm>
#include <stdexcept>
#include "visitedArray.hpp"

namespace chm {
	VisitedArrayTask::VisitedArrayTask(const uint itemCount, const uint queryCount, const uint seed, const bool equalSeed) {
		if(!itemCount)
			throwZeroItems("items");
		if(!queryCount)
			throwZeroItems("queries");

		const auto maxElem = itemCount - 1;
		this->items = generateData<uint>(size_t(itemCount), 0, maxElem, seed);
		this->queries = generateData<uint>(size_t(queryCount), 0, maxElem, equalSeed ? seed : seed + 1);
	}

	VisitedArrayResult::VisitedArrayResult(const size_t queryCount) : VectorResult<VisitedArrayResult, uint>(queryCount) {}

	VisitedArrayResult::VisitedArrayResult(const std::vector<uint>& items, const std::vector<uint>& queries)
		: VectorResult<VisitedArrayResult, uint>(queries.size()) {

		std::vector<uint> itemsCopy(items);
		std::sort(itemsCopy.begin(), itemsCopy.end());

		for(const auto& q : queries)
			this->items.emplace_back((uint)(std::binary_search(itemsCopy.begin(), itemsCopy.end(), q)));
	}

	VisitedArrayArgs::Result::Opt VisitedArrayArgs::getCorrectRes() const {
		return std::make_optional<VisitedArrayArgs::Result>(this->items, this->queries);
	}

	size_t VisitedArrayArgs::getQueryCount() const {
		return this->queries.size();
	}

	VisitedArrayArgs::VisitedArrayArgs(const VisitedArrayTask& t) : VectorNoSetupArgs<uint, VisitedArrayResult>(t.items), queries(t.queries) {}

	void throwZeroItems(const std::string& items) {
		std::stringstream s;
		s << "There can't be zero " << items << '.';
		throw std::runtime_error(s.str());
	}
}
