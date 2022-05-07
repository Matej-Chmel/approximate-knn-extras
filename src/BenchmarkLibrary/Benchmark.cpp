#include "Benchmark.hpp"

namespace chm {
	void SplitTime::print(std::ostream& s) const {
		s << this->seconds.count() << '.' << this->millis.count() << '.' << this->micros.count() << '.' << this->nanos.count();
	}

	SplitTime::SplitTime(const chr::nanoseconds& elapsed) {
		chr::nanoseconds elapsedCopy = elapsed;

		splitTime(elapsedCopy, this->seconds);
		splitTime(elapsedCopy, this->millis);
		splitTime(elapsedCopy, this->micros);
		this->nanos = elapsedCopy;
	}

	void checkNameNotEmpty(const std::string& className, const std::string& name) {
		if(name.empty()) {
			std::stringstream s;
			s << "Name for an object of class \"" << className << "\" can't be empty.";
			throw std::runtime_error(s.str());
		}
	}

	std::ostream& operator<<(std::ostream& s, const SplitTime& time) {
		time.print(s);
		return s;
	}
}
