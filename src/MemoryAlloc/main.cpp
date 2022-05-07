#include <cstring>
#include <iostream>
#include <stdlib.h>
#include "BenchmarkLibrary/Benchmark.hpp"

namespace chm {
	struct MemoryAllocArgs {
		const size_t level;
		const size_t maxElements;
		const size_t mMax;

		MemoryAllocArgs(const size_t level, const size_t maxElements, const size_t mMax) : level(level), maxElements(maxElements), mMax(mMax) {}
	};

	void runMalloc(const MemoryAllocArgs& args) {
		auto linkLists = (char**)malloc(sizeof(void*) * args.maxElements);

		if(!linkLists)
			throw std::runtime_error("Not enough memory for link lists.");

		const auto maxLinks = (args.mMax + 1) * sizeof(unsigned int) * args.level;

		for(size_t i = 0; i < args.maxElements; i++) {
			linkLists[i] = (char*)malloc(maxLinks);

			if(!linkLists[i])
				throw std::runtime_error("Not enough memory for one link list.");

			memset(linkLists[i], 0, maxLinks);
		}

		for(size_t i = 0; i < args.maxElements; i++)
			free(linkLists[i]);
		free(linkLists);
	}

	void runVector(const MemoryAllocArgs& args) {
		std::vector<std::vector<unsigned int>> linkLists(args.maxElements);
		const auto maxLinks = (args.mMax + 1) * args.level;

		for(auto& list : linkLists)
			list.resize(maxLinks, 0);
	}
}

int main() {
	using namespace chm;

	try {
		BenchmarkSuite<MemoryAllocArgs>(MemoryAllocArgs(5, 50000, 32), "Memory allocation test")
			.add(runMalloc, "malloc")
			.add(runVector, "vector")
			.repeat(100)
			.print(std::cout, "\n\n");
	} catch(const std::exception& e) {
		std::cerr << "[ERROR] " << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
