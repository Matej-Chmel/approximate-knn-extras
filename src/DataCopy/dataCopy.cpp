#include <algorithm>
#include <cstring>
#include <limits>
#include "dataCopy.hpp"
#include "DataGeneratorLibrary/dataGenerator.hpp"

namespace chm {
	DataCopyResult::DataCopyResult(const size_t count, const size_t dim) {
		this->data.resize(count * dim);
	}

	DataCopyResult::DataCopyResult(const std::vector<float>& data) : data(data) {}

	float* DataCopyResult::getData() {
		return this->data.data();
	}

	bool DataCopyResult::operator!=(const DataCopyResult& o) const {
		if(this->data.size() != o.data.size())
			return true;

		const auto len = this->data.size();

		for(size_t i = 0; i < len; i++)
			if(this->data[i] != o.data[i])
				return true;
		return false;
	}

	DataCopyArgs::DataCopyArgs(const size_t count, const size_t dim) : count(count), dim(dim) {
		if(!count || !dim)
			throw std::runtime_error("DataCopyArgs can't have empty data vector.");

		this->data = generateData<float>(count * dim, -1000000.f, 1000000.f, 100);
	}

	DataCopyArgs::Result DataCopyArgs::getCorrectRes() const {
		return DataCopyArgs::Result(this->data);
	}

	const float* const DataCopyArgs::getData() const {
		return this->data.data();
	}

	DataCopyArgs::Result runCopyAtOnce(const DataCopyArgs& args) {
		DataCopyArgs::Result res(args.count, args.dim);
		const auto data = args.getData();
		std::copy(data, data + args.count * args.dim, res.getData());
		return res;
	}

	DataCopyArgs::Result runStagedCopy(const DataCopyArgs& args) {
		auto argsData = args.getData();
		const auto dataSize = args.dim * sizeof(float);
		const auto endPtr = argsData + args.count * args.dim;
		DataCopyArgs::Result res(args.count, args.dim);
		auto resData = res.getData();

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
