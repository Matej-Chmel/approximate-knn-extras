#pragma once
#include "BenchmarkLibrary/Result.hpp"

namespace chm {
	class DataCopyResult : public VectorResult<DataCopyResult, float> {
	public:
		DataCopyResult() = default;
		DataCopyResult(const std::vector<float>& data);
		DataCopyResult(const size_t count, const size_t dim);
	};

	struct DataCopyArgs : public NoSetupArgs {
	public:
		using Result = DataCopyResult;

		const size_t count;
		const size_t dim;

		DataCopyArgs(const size_t count, const size_t dim);
		Result::Opt getCorrectRes() const;
		const float* const getData() const;

	private:
		std::vector<float> data;
	};

	DataCopyArgs::Result::Opt runCopyAtOnce(const DataCopyArgs& args);
	DataCopyArgs::Result::Opt runStagedCopy(const DataCopyArgs& args);
}
