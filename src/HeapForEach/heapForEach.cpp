#include <algorithm>
#include "heapForEach.hpp"

namespace chm {
	HeapForEachResult::HeapForEachResult(const size_t maxLen) : VectorResult<HeapForEachResult, uint>(maxLen) {}

	HeapForEachResult::HeapForEachResult(const std::vector<Node>& nodes) : VectorResult<HeapForEachResult, uint>(nodes.size()) {
		for(const auto& n : nodes)
			this->items.emplace_back(n.id);

		std::sort(this->items.begin(), this->items.end());
	}

	bool HeapForEachResult::operator!=(const HeapForEachResult& o) const {
		std::vector<uint> itemsCopy(this->items);
		std::vector<uint> otherItemsCopy(o.items);

		std::sort(itemsCopy.begin(), itemsCopy.end());
		std::sort(otherItemsCopy.begin(), otherItemsCopy.end());
		return itemsCopy != otherItemsCopy;
	}

	HeapForEachArgs::HeapForEachArgs(const std::vector<Node>& nodes) : VectorArgs<Node, HeapForEachResult, bool>(nodes) {}

	void HeapForEachArgs::setup(const bool& prepareQueue) {
		if(prepareQueue) {
			this->queue = PriorityQueue();

			for(const auto& n : this->items)
				this->queue.emplace(n.distance, n.id);
		} else {
			this->vec.clear();
			this->vec.reserve(this->getItemCount());

			for(const auto& n : this->items) {
				this->vec.emplace_back(n.distance, n.id);
				std::push_heap(this->vec.begin(), this->vec.end(), NearHeapComparator());
			}
		}
	}

	HeapForEachArgs::Result::Opt iterateQueue(HeapForEachArgs& args) {
		auto res = std::make_optional<HeapForEachArgs::Result>(args.getItemCount());

		while(!args.queue.empty()) {
			res->add(args.queue.top().id);
			args.queue.pop();
		}

		return res;
	}

	HeapForEachArgs::Result::Opt iterateVector(HeapForEachArgs& args) {
		auto res = std::make_optional<HeapForEachArgs::Result>(args.getItemCount());

		for(const auto& n : args.vec)
			res->add(n.id);

		return res;
	}
}
