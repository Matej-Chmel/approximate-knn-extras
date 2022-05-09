#include <iostream>
#include "BenchmarkLibrary/Benchmark.hpp"
#include "dataCopy.hpp"

int main() {
	using namespace chm;

	return catchAllExceptions([]() {
		BenchmarkSuite<>()
			.add()
			.add()
			.repeat(1)
			.print(std::cout, "\n\n");
	});
}
