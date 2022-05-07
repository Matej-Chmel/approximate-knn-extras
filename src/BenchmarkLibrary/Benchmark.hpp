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

	template<class T>
	class Benchmark {
	public:
		using Func = std::function<void(const T&)>;

		Benchmark(const Func& func, const std::string& name);
		void print(std::ostream& s) const;
		void run(const T& args);

	private:
		bool benchmarked;
		const std::function<void(const T&)> func;
		chr::nanoseconds minTime;
		const std::string name;

		void setIfMinimal(const chr::nanoseconds& elapsed);
	};

	template<class T>
	class BenchmarkSuite {
	public:
		BenchmarkSuite<T> add(const typename Benchmark<T>::Func& func, const std::string& name);
		BenchmarkSuite(const T& args, const std::string& name);
		BenchmarkSuite<T> print(std::ostream& s) const;
		BenchmarkSuite<T> print(std::ostream& s, const std::string& endStr) const;
		BenchmarkSuite<T> repeat(const size_t r);

	private:
		const T args;
		std::vector<Benchmark<T>> benchmarks;
		const std::string name;

		void checkNotEmpty() const;
	};

	void checkNameNotEmpty(const std::string& className, const std::string& name);
	std::ostream& operator<<(std::ostream& s, const SplitTime& time);
	template<class T> std::ostream& operator<<(std::ostream& s, const Benchmark<T>& benchmark);
	template<class T> std::ostream& operator<<(std::ostream& s, const BenchmarkSuite<T> suite);
	template<class T> void splitTime(chr::nanoseconds& elapsed, T& res);

	template<class T>
	inline Benchmark<T>::Benchmark(const Func& func, const std::string& name) : benchmarked(false), func(func), minTime(0), name(name) {
		checkNameNotEmpty("Benchmark", this->name);
	}

	template<class T>
	inline void Benchmark<T>::print(std::ostream& s) const {
		if(!this->benchmarked) {
			std::stringstream strStream;
			strStream << "Benchmark \"" << this->name << "\" wasn't benchmarked yet.";
			throw std::runtime_error(strStream.str());
		}

		s << "Minimum time of \"" << this->name << "\": " << SplitTime(this->minTime);
	}

	template<class T>
	inline void Benchmark<T>::run(const T& args) {
		const auto timeBegin = chr::steady_clock::now();
		this->func(args);
		const auto timeEnd = chr::steady_clock::now();
		this->setIfMinimal(chr::duration_cast<chr::nanoseconds>(timeEnd - timeBegin));
	}

	template<class T>
	inline void Benchmark<T>::setIfMinimal(const chr::nanoseconds& elapsed) {
		if(!this->benchmarked) {
			this->benchmarked = true;
			this->minTime = elapsed;
		} else if(elapsed < this->minTime)
			this->minTime = elapsed;
	}

	template<class T>
	inline BenchmarkSuite<T> BenchmarkSuite<T>::add(const typename Benchmark<T>::Func& func, const std::string& name) {
		this->benchmarks.emplace_back(func, name);
		return *this;
	}

	template<class T>
	inline BenchmarkSuite<T>::BenchmarkSuite(const T& args, const std::string& name) : args(args), name(name) {
		checkNameNotEmpty("BenchmarkSuite", this->name);
	}

	template<class T>
	inline BenchmarkSuite<T> BenchmarkSuite<T>::print(std::ostream& s) const {
		this->checkNotEmpty();

		const auto lastIdx = this->benchmarks.size() - 1;
		s << this->name << '\n';

		for(size_t i = 0; i < lastIdx; i++) {
			this->benchmarks[i].print(s);
			s << '\n';
		}

		this->benchmarks[lastIdx].print(s);
		return *this;
	}

	template<class T>
	inline BenchmarkSuite<T> BenchmarkSuite<T>::print(std::ostream& s, const std::string& endStr) const {
		BenchmarkSuite<T>::print(s);
		s << endStr;
		return *this;
	}

	template<class T>
	inline BenchmarkSuite<T> BenchmarkSuite<T>::repeat(const size_t r) {
		this->checkNotEmpty();

		for(size_t i = 0; i < r; i++)
			for(auto& b : this->benchmarks)
				b.run(this->args);

		return *this;
	}

	template<class T>
	inline void BenchmarkSuite<T>::checkNotEmpty() const {
		if(this->benchmarks.empty()) {
			std::stringstream strStream;
			strStream << "No benchmarks to run for \"" << this->name << "\".";
			throw std::runtime_error(strStream.str());
		}
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& s, const Benchmark<T>& benchmark) {
		benchmark.print(s);
		return s;
	}

	template<class T>
	inline std::ostream& operator<<(std::ostream& s, const BenchmarkSuite<T> suite) {
		(void)suite.print(s);
		return s;
	}

	template<class T>
	inline void splitTime(chr::nanoseconds& elapsed, T& res) {
		res = chr::duration_cast<T>(elapsed);
		elapsed -= res;
	}
}
