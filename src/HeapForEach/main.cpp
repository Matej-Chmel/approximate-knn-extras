#include <iostream>
#include "BenchmarkLibrary/Benchmark.hpp"
#include "heapForEach.hpp"

int main() {
	using namespace chm;

	return catchAllExceptions([]() {
		HeapForEachArgs args(generateNodes(100, 0.f, 1000000.f, 30));

		BenchmarkSuite<HeapForEachArgs>(args, "Benchmark of heap iteration methods", args.getCorrectRes())
			.add(iterateQueue, "iterate queue", true)
			.add(iterateVector, "iterate vector", false)
			.repeat(500000)
			.print(std::cout, "\n\n");
	});
}
