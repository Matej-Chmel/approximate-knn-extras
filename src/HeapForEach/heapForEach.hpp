#pragma once
#include <queue>
#include "BenchmarkLibrary/Args.hpp"
#include "DataGeneratorLibrary/dataGenerator.hpp"

namespace chm {
	struct HeapForEachResult : public VectorResult<HeapForEachResult, uint> {
		HeapForEachResult(const size_t maxLen);
		HeapForEachResult(const std::vector<Node>& nodes);
		bool operator!=(const HeapForEachResult& o) const;
	};

	class HeapForEachArgs : public VectorArgs<Node, HeapForEachResult, bool> {
	public:
		using PriorityQueue = std::priority_queue<Node, std::vector<Node>, NearHeapComparator>;
		PriorityQueue queue;
		std::vector<Node> vec;

		HeapForEachArgs(const std::vector<Node>& nodes);
		void setup(const bool& prepareQueue);
	};

	HeapForEachArgs::Result::Opt iterateQueue(HeapForEachArgs& args);
	HeapForEachArgs::Result::Opt iterateVector(HeapForEachArgs& args);
}
