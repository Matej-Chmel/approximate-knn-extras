#include <sstream>
#include "Result.hpp"

namespace chm {
	bool UncomparableResult::operator!=(const UncomparableResult& o) const {
		throw std::runtime_error("Objects of class \"UncomparableResult\" can't be compared.");
		return true;
	}

	void NoSetupArgs::setup(const SetupTask&) {
		throwNotImplemented("NoSetupArgs", "setup");
	}

	void throwNotImplemented(const std::string& className, const std::string& methodName) {
		std::stringstream s;
		s << "Method \"" << className << "::" << methodName << "\" not implemented.";
		throw std::runtime_error(s.str());
	}
}
