#include <iostream>
#include "BenchmarkLibrary/Benchmark.hpp"
#include "dataCopy.hpp"

int main() {
	using namespace chm;

	return catchAllExceptions([]() {
		DataCopyArgs args(250000, 25);

		BenchmarkSuite<DataCopyArgs>(args, "Data copy benchmark", args.getCorrectRes())
			.add(runStagedCopy, "staged copy")
			.add(runCopyAtOnce, "copy at once")
			.repeat(200)
			.print(std::cout, "\n\n");
	});
}
