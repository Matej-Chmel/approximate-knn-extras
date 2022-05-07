#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <vector>
namespace chr = std::chrono;

using RunFunc = std::function<void(const size_t, const size_t, const size_t)>;

void runMalloc(const size_t level, const size_t listCount, const size_t mMax) {
	auto linkLists = (char**)malloc(sizeof(void*) * listCount);

	if(!linkLists)
		throw std::runtime_error("Not enough memory for link lists.");

	const auto maxLinks = (mMax + 1) * sizeof(unsigned int) * level;

	for(size_t i = 0; i < listCount; i++) {
		linkLists[i] = (char*)malloc(maxLinks);

		if(!linkLists[i])
			throw std::runtime_error("Not enough memory for one link list.");

		memset(linkLists[i], 0, maxLinks);
	}

	for(size_t i = 0; i < listCount; i++)
		free(linkLists[i]);
	free(linkLists);
}

void runVector(const size_t level, const size_t listCount, const size_t mMax) {
	std::vector<std::vector<unsigned int>> linkLists(listCount);
	const auto maxLinks = (mMax + 1) * level;

	for(auto& list : linkLists)
		list.resize(maxLinks, 0);
}

template<typename T>
void splitTime(chr::nanoseconds& elapsed, T& res) {
	res = chr::duration_cast<T>(elapsed);
	elapsed -= res;
}

struct SplitTimePoint {
	chr::seconds seconds;
	chr::milliseconds millis;
	chr::microseconds micros;
	chr::nanoseconds nanos;

	void print(std::ostream& s) const {
		s << this->seconds.count() << '.' << this->millis.count() << '.' << this->micros.count() << '.' << this->nanos.count();
	}

	SplitTimePoint(const chr::nanoseconds& elapsed) {
		chr::nanoseconds elapsedCopy = elapsed;

		splitTime(elapsedCopy, this->seconds);
		splitTime(elapsedCopy, this->millis);
		splitTime(elapsedCopy, this->micros);
		this->nanos = elapsedCopy;
	}
};

std::ostream& operator<<(std::ostream& s, const SplitTimePoint& timePoint) {
	timePoint.print(s);
	return s;
}

class Benchmark {
	bool benchmarkedMalloc;
	bool benchmarkedVector;
	const size_t level;
	const size_t listCount;
	chr::nanoseconds minMalloc;
	chr::nanoseconds minVector;
	const size_t mMax;

	void benchRunFunc(bool& benchmarked, const RunFunc& f, chr::nanoseconds& minTime) {
		const auto timeBegin = chr::steady_clock::now();
		f(this->level, this->listCount, this->mMax);
		const auto timeEnd = chr::steady_clock::now();
		const auto elapsed = chr::duration_cast<chr::nanoseconds>(timeEnd - timeBegin);

		if(!benchmarked) {
			benchmarked = true;
			minTime = elapsed;
		} else if(elapsed < minTime)
			minTime = elapsed;
	}

	void benchMalloc() {
		this->benchRunFunc(this->benchmarkedMalloc, runMalloc, this->minMalloc);
	}

	void benchVector() {
		this->benchRunFunc(this->benchmarkedVector, runVector, this->minVector);
	}

public:
	Benchmark(const size_t level, const size_t listCount, const size_t mMax)
		: benchmarkedMalloc(false), benchmarkedVector(false), level(level), listCount(listCount),
		minMalloc(0), minVector(0), mMax(mMax) {
	}

	Benchmark& benchAll(const size_t repeats) {
		for(size_t i = 0; i < repeats; i++) {
			this->benchMalloc();
			this->benchVector();
		}
		return *this;
	}

	void print(std::ostream& s) const {
		s << "Minimum malloc time: " << SplitTimePoint(this->minMalloc) << "\nMinimum vector time: " << SplitTimePoint(this->minVector) << '\n';
	}
};

int main() {
	Benchmark(5, 50000, 32).benchAll(100).print(std::cout);
	return 0;
}
