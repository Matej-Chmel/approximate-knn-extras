#include <iostream>
#include "BenchmarkLibrary/Benchmark.hpp"
#include "heap.hpp"

int main() {
	using namespace chm;

	return catchAllExceptions([]() {
		HeapArgs args(generateNodes(500, 0.f, 5000.f, 10));

		BenchmarkSuite<HeapArgs>(args, "Heap implementation benchmark", args.getCorrectRes())
			.add(runPriorityQueue, "priority queue")
			.add(runPushPopHeap<false>, "push-pop")
			.add(runPushPopHeap<true>, "reserve-push-pop")
			.repeat(100000)
			.print(std::cout, "\n\n");
	});
}
