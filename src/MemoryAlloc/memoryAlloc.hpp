#pragma once
#include "BenchmarkLibrary/Result.hpp"

namespace chm {
	struct MemoryAllocArgs : public UncomparableArgs {
		const size_t level;
		const size_t maxElements;
		const size_t mMax;

		MemoryAllocArgs(const size_t level, const size_t maxElements, const size_t mMax);
	};

	MemoryAllocArgs::Result::Opt runMalloc(const MemoryAllocArgs& args);
	MemoryAllocArgs::Result::Opt runVector(const MemoryAllocArgs& args);
}
