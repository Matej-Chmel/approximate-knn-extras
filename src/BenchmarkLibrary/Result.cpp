#include <stdexcept>
#include "Result.hpp"

namespace chm {
	bool UncomparableResult::operator!=(const UncomparableResult& o) const {
		throw std::runtime_error("Objects of class \"UncomparableResult\" can't be compared.");
		return true;
	}

	void NoSetupArgs::setup(const SetupTask&) {}
}
