#include <iostream>
#include "BenchmarkLibrary/Benchmark.hpp"
#include "visitedArray.hpp"

int main() {
	using namespace chm;

	return catchAllExceptions([]() {
		VisitedArrayArgs args(VisitedArrayTask(1000000, 10000, 50));

		BenchmarkSuite<VisitedArrayArgs>(args, args.getCorrectRes(), "Visited array benchmark")
			.add(runVisitedArray<unsigned char>, "plain array")
			.add(runVisitedArray<bool>, "bit array")
			.repeat(2000)
			.print(std::cout, "\n\n");

		std::cout << "Size of plain array: " << args.getElementCount() * 8 <<
			" bits\nSize of bit array: " << args.getElementCount() << " bits.\n";
	});
}
