#include <iostream>
#include "BenchmarkLibrary/Benchmark.hpp"
#include "memoryAlloc.hpp"

int main() {
	using namespace chm;

	return catchAllExceptions([]() {
		BenchmarkSuite<MemoryAllocArgs>(MemoryAllocArgs(5, 50000, 32), "Memory allocation benchmark")
			.add(runMalloc, "malloc")
			.add(runVector, "vector")
			.repeat(100)
			.print(std::cout, "\n\n");
	});
}
