#pragma once

namespace chm {
	struct MemoryAllocResult {
		bool operator!=(const MemoryAllocResult&) const;
	};

	struct MemoryAllocArgs {
		using Result = MemoryAllocResult;

		const size_t level;
		const size_t maxElements;
		const size_t mMax;

		MemoryAllocArgs(const size_t level, const size_t maxElements, const size_t mMax);
	};

	MemoryAllocArgs::Result runMalloc(const MemoryAllocArgs& args);
	MemoryAllocArgs::Result runVector(const MemoryAllocArgs& args);
}
