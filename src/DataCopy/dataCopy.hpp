#pragma once
#include "BenchmarkLibrary/Args.hpp"

namespace chm {
	struct DataCopyResult : public VectorResult<DataCopyResult, float> {
		DataCopyResult() = default;
		DataCopyResult(const std::vector<float>& items);
		DataCopyResult(const size_t count, const size_t dim);
	};

	struct DataCopyArgs : public VectorNoSetupArgs<float, DataCopyResult> {
		const size_t count;
		const size_t dim;

		DataCopyArgs(const size_t count, const size_t dim);
	};

	DataCopyArgs::Result::Opt runCopyAtOnce(const DataCopyArgs& args);
	DataCopyArgs::Result::Opt runStagedCopy(const DataCopyArgs& args);
}
