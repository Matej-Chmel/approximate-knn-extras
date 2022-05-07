#pragma once
#include <chrono>
#include <functional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace chm {
	namespace chr = std::chrono;

	class SplitTime {
	public:
		void print(std::ostream& s) const;
		SplitTime(const chr::nanoseconds& elapsed);

	private:
		chr::seconds seconds;
		chr::milliseconds millis;
		chr::microseconds micros;
		chr::nanoseconds nanos;
	};

	class Timer {
	public:
		chr::nanoseconds getElapsed() const;
		Timer();

	private:
		chr::steady_clock::time_point start;
	};

	template<class Args>
	class Benchmark {
	public:
		using Func = std::function<typename Args::Result(const Args&)>;

		Benchmark(const Func& func, const std::string& name);
		void print(std::ostream& s) const;
		void run(const Args& args, const bool compareRes, const typename Args::Result& expectedRes);

	private:
		bool benchmarked;
		const Func func;
		chr::nanoseconds minTime;
		const std::string name;

		void checkResults(const typename Args::Result& actual, const typename Args::Result& expected) const;
		void setIfMinimal(const chr::nanoseconds& elapsed);
	};

	template<class Args>
	class BenchmarkSuite {
	public:
		BenchmarkSuite<Args> add(const typename Benchmark<Args>::Func& func, const std::string& name);
		BenchmarkSuite(const Args& args, const std::string& name);
		BenchmarkSuite(const Args& args, const typename Args::Result& correctRes, const std::string& name);
		BenchmarkSuite<Args> print(std::ostream& s) const;
		BenchmarkSuite<Args> print(std::ostream& s, const std::string& endStr) const;
		BenchmarkSuite<Args> repeat(const size_t r);

	private:
		const Args args;
		std::vector<Benchmark<Args>> benchmarks;
		const bool compareRes;
		const typename Args::Result correctRes;
		chr::nanoseconds elapsedTotal;
		const std::string name;

		void checkNotEmpty() const;
	};

	int catchAllExceptions(const std::function<void()>& f);
	void checkNameNotEmpty(const std::string& className, const std::string& name);
	std::ostream& operator<<(std::ostream& s, const SplitTime& time);
	template<class Args> std::ostream& operator<<(std::ostream& s, const Benchmark<Args>& benchmark);
	template<class Args> std::ostream& operator<<(std::ostream& s, const BenchmarkSuite<Args> suite);
	template<class T> void splitTime(chr::nanoseconds& elapsed, T& res);

	template<class Args>
	inline Benchmark<Args>::Benchmark(const Func& func, const std::string& name) : benchmarked(false), func(func), minTime(0), name(name) {
		checkNameNotEmpty("Benchmark", this->name);
	}

	template<class Args>
	inline void Benchmark<Args>::print(std::ostream& s) const {
		if(!this->benchmarked) {
			std::stringstream strStream;
			strStream << "Benchmark \"" << this->name << "\" wasn't benchmarked yet.";
			throw std::runtime_error(strStream.str());
		}

		s << "Minimum time of \"" << this->name << "\": " << SplitTime(this->minTime);
	}

	template<class Args>
	inline void Benchmark<Args>::run(const Args& args, const bool compareRes, const typename Args::Result& expectedRes) {
		Timer timer{};
		const auto actualRes = this->func(args);
		const auto elapsed = timer.getElapsed();

		if(compareRes)
			this->checkResults(actualRes, expectedRes);
		this->setIfMinimal(elapsed);
	}

	template<class Args>
	inline void Benchmark<Args>::checkResults(const typename Args::Result& actual, const typename Args::Result& expected) const {
		if(actual != expected) {
			std::stringstream s;
			s << "Results mismatch for \"" << this->name << "\".";
			throw std::runtime_error(s.str());
		}
	}

	template<class Args>
	inline void Benchmark<Args>::setIfMinimal(const chr::nanoseconds& elapsed) {
		if(!this->benchmarked) {
			this->benchmarked = true;
			this->minTime = elapsed;
		} else if(elapsed < this->minTime)
			this->minTime = elapsed;
	}

	template<class Args>
	inline BenchmarkSuite<Args> BenchmarkSuite<Args>::add(const typename Benchmark<Args>::Func& func, const std::string& name) {
		this->benchmarks.emplace_back(func, name);
		return *this;
	}

	template<class Args>
	inline BenchmarkSuite<Args>::BenchmarkSuite(const Args& args, const std::string& name)
		: args(args), compareRes(false), correctRes{}, elapsedTotal(0), name(name) {

		checkNameNotEmpty("BenchmarkSuite", this->name);
	}

	template<class Args>
	inline BenchmarkSuite<Args>::BenchmarkSuite(const Args& args, const typename Args::Result& correctRes, const std::string& name)
		: args(args), compareRes(true), correctRes(correctRes), elapsedTotal(0), name(name) {

		checkNameNotEmpty("BenchmarkSuite", this->name);
	}

	template<class Args>
	inline BenchmarkSuite<Args> BenchmarkSuite<Args>::print(std::ostream& s) const {
		this->checkNotEmpty();

		const auto lastIdx = this->benchmarks.size() - 1;
		s << this->name << "\nTotal time elapsed while benchmarking: " << SplitTime(this->elapsedTotal) << "\n\n";

		for(size_t i = 0; i < lastIdx; i++) {
			this->benchmarks[i].print(s);
			s << '\n';
		}

		this->benchmarks[lastIdx].print(s);
		return *this;
	}

	template<class Args>
	inline BenchmarkSuite<Args> BenchmarkSuite<Args>::print(std::ostream& s, const std::string& endStr) const {
		BenchmarkSuite<Args>::print(s);
		s << endStr;
		return *this;
	}

	template<class Args>
	inline BenchmarkSuite<Args> BenchmarkSuite<Args>::repeat(const size_t r) {
		this->checkNotEmpty();

		Timer timer{};

		for(size_t i = 0; i < r; i++)
			for(auto& b : this->benchmarks)
				b.run(this->args, this->compareRes, this->correctRes);

		this->elapsedTotal += timer.getElapsed();
		return *this;
	}

	template<class Args>
	inline void BenchmarkSuite<Args>::checkNotEmpty() const {
		if(this->benchmarks.empty()) {
			std::stringstream strStream;
			strStream << "No benchmarks to run for \"" << this->name << "\".";
			throw std::runtime_error(strStream.str());
		}
	}

	template<class Args>
	inline std::ostream& operator<<(std::ostream& s, const Benchmark<Args>& benchmark) {
		benchmark.print(s);
		return s;
	}

	template<class Args>
	inline std::ostream& operator<<(std::ostream& s, const BenchmarkSuite<Args> suite) {
		(void)suite.print(s);
		return s;
	}

	template<class T>
	inline void splitTime(chr::nanoseconds& elapsed, T& res) {
		res = chr::duration_cast<T>(elapsed);
		elapsed -= res;
	}
}
