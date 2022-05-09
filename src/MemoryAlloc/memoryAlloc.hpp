#pragma once
#include "BenchmarkLibrary/Result.hpp"

namespace chm {
	struct MemoryAllocArgs {
		using Result = UncomparableResult;

		const size_t level;
		const size_t maxElements;
		const size_t mMax;

		MemoryAllocArgs(const size_t level, const size_t maxElements, const size_t mMax);
	};

	MemoryAllocArgs::Result runMalloc(const MemoryAllocArgs& args);
	MemoryAllocArgs::Result runVector(const MemoryAllocArgs& args);
}
