#pragma once
#include <chrono>
#include <functional>
#include <optional>
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
		using Result = std::optional<typename Args::Result>;
		using Func = std::function<Result(const Args&)>;
		using SetupTask = std::optional<typename Args::SetupTask>;

		Benchmark(const Func& func, const std::string& name, const SetupTask& setupTask = std::nullopt);
		void print(std::ostream& s) const;
		void run(Args& args, const Result& expectedRes);

	private:
		const Func func;
		std::optional<chr::nanoseconds> minTime;
		const std::string name;
		const SetupTask setupTask;

		void checkResults(const Result& actual, const Result& expected) const;
		void setIfMinimal(const chr::nanoseconds& elapsed);
	};

	template<class Args>
	class BenchmarkSuite {
	public:
		using Func = typename Benchmark<Args>::Func;
		using Result = typename Benchmark<Args>::Result;
		using SetupTask = typename Benchmark<Args>::SetupTask;

		BenchmarkSuite<Args> add(const Func& func, const std::string& name, const SetupTask& setupTask = std::nullopt);
		BenchmarkSuite(Args args, const std::string& name, const Result& correctRes = std::nullopt);
		BenchmarkSuite<Args> print(std::ostream& s, const std::optional<std::string>& endStr = std::nullopt) const;
		BenchmarkSuite<Args> repeat(const size_t r);

	private:
		Args args;
		std::vector<Benchmark<Args>> benchmarks;
		const Result correctRes;
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
	inline Benchmark<Args>::Benchmark(const Func& func, const std::string& name, const SetupTask& setupTask)
		: func(func), minTime(std::nullopt), name(name), setupTask(setupTask) {

		checkNameNotEmpty("Benchmark", this->name);
	}

	template<class Args>
	inline void Benchmark<Args>::print(std::ostream& s) const {
		if(!this->minTime) {
			std::stringstream strStream;
			strStream << "Benchmark \"" << this->name << "\" wasn't benchmarked yet.";
			throw std::runtime_error(strStream.str());
		}

		s << "Minimum time of \"" << this->name << "\": " << SplitTime(*this->minTime);
	}

	template<class Args>
	inline void Benchmark<Args>::run(Args& args, const Result& expectedRes) {
		if(this->setupTask)
			args.setup(*this->setupTask);

		Timer timer{};
		const auto actualRes = this->func(args);
		const auto elapsed = timer.getElapsed();

		this->checkResults(actualRes, expectedRes);
		this->setIfMinimal(elapsed);
	}

	template<class Args>
	inline void Benchmark<Args>::checkResults(const Result& actual, const Result& expected) const {
		if(actual != expected) {
			std::stringstream s;
			s << "Results mismatch for \"" << this->name << "\".";
			throw std::runtime_error(s.str());
		}
	}

	template<class Args>
	inline void Benchmark<Args>::setIfMinimal(const chr::nanoseconds& elapsed) {
		if(!this->minTime || elapsed < *this->minTime)
			this->minTime = elapsed;
	}

	template<class Args>
	inline BenchmarkSuite<Args> BenchmarkSuite<Args>::add(const Func& func, const std::string& name, const SetupTask& setupTask) {
		this->benchmarks.emplace_back(func, name, setupTask);
		return *this;
	}

	template<class Args>
	inline BenchmarkSuite<Args>::BenchmarkSuite(Args args, const std::string& name, const Result& correctRes)
		: args(args), correctRes(correctRes), elapsedTotal(0), name(name) {

		checkNameNotEmpty("BenchmarkSuite", this->name);
	}

	template<class Args>
	inline BenchmarkSuite<Args> BenchmarkSuite<Args>::print(std::ostream& s, const std::optional<std::string>& endStr) const {
		this->checkNotEmpty();

		const auto lastIdx = this->benchmarks.size() - 1;
		s << this->name << "\nTotal time elapsed while benchmarking: " << SplitTime(this->elapsedTotal) << "\n\n";

		for(size_t i = 0; i < lastIdx; i++) {
			this->benchmarks[i].print(s);
			s << '\n';
		}

		this->benchmarks[lastIdx].print(s);

		if(endStr)
			s << *endStr;

		return *this;
	}

	template<class Args>
	inline BenchmarkSuite<Args> BenchmarkSuite<Args>::repeat(const size_t r) {
		this->checkNotEmpty();

		Timer timer{};

		for(size_t i = 0; i < r; i++)
			for(auto& b : this->benchmarks)
				b.run(this->args, this->correctRes);

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
