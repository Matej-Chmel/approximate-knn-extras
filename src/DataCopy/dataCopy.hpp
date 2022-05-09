#pragma once
#include "BenchmarkLibrary/Result.hpp"

namespace chm {
	class DataCopyResult : public VectorResult<float> {
	public:
		DataCopyResult() = default;
		DataCopyResult(const std::vector<float>& data);
		DataCopyResult(const size_t count, const size_t dim);
	};

	struct DataCopyArgs {
	public:
		using Result = DataCopyResult;

		const size_t count;
		const size_t dim;

		DataCopyArgs(const size_t count, const size_t dim);
		Result getCorrectRes() const;
		const float* const getData() const;

	private:
		std::vector<float> data;
	};

	DataCopyArgs::Result runCopyAtOnce(const DataCopyArgs& args);
	DataCopyArgs::Result runStagedCopy(const DataCopyArgs& args);
}
