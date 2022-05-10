#include <algorithm>
#include <cstring>
#include <limits>
#include "dataCopy.hpp"
#include "DataGeneratorLibrary/dataGenerator.hpp"

namespace chm {
	DataCopyResult::DataCopyResult(const std::vector<float>& data) : VectorResult<DataCopyResult, float>(data) {}

	DataCopyResult::DataCopyResult(const size_t count, const size_t dim) : VectorResult<DataCopyResult, float>(count* dim, false) {}

	DataCopyArgs::DataCopyArgs(const size_t count, const size_t dim) : count(count), dim(dim) {
		if(!count || !dim)
			throw std::runtime_error("DataCopyArgs can't have empty data vector.");

		this->data = generateData<float>(count * dim, -1000000.f, 1000000.f, 100);
	}

	DataCopyArgs::Result::Opt DataCopyArgs::getCorrectRes() const {
		return std::make_optional<DataCopyArgs::Result>(this->data);
	}

	const float* const DataCopyArgs::getData() const {
		return this->data.data();
	}

	DataCopyArgs::Result::Opt runCopyAtOnce(const DataCopyArgs& args) {
		auto res = std::make_optional<DataCopyArgs::Result>(args.count, args.dim);
		const auto data = args.getData();
		std::copy(data, data + args.count * args.dim, res->getData());
		return res;
	}

	DataCopyArgs::Result::Opt runStagedCopy(const DataCopyArgs& args) {
		auto argsData = args.getData();
		const auto dataSize = args.dim * sizeof(float);
		const auto endPtr = argsData + args.count * args.dim;
		auto res = std::make_optional<DataCopyArgs::Result>(args.count, args.dim);
		auto resData = res->getData();

		(void)memcpy(resData, argsData, dataSize);
		argsData += args.dim;

		while(argsData != endPtr) {
			resData += args.dim;
			(void)memcpy(resData, argsData, dataSize);
			argsData += args.dim;
		}

		return res;
	}
}
