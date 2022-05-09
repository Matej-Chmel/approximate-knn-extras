#pragma once
#include <vector>

namespace chm {
	class DataCopyResult {
	public:
		DataCopyResult(const size_t count, const size_t dim);
		DataCopyResult(const std::vector<float>& data);
		float* getData();
		bool operator!=(const DataCopyResult& o) const;

	private:
		std::vector<float> data;
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
