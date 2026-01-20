#include "vline.hpp"
#include <stdexcept>

topit::VLine::VLine(p_t id, size_t len) :
	IDraw(),
	segment{ id },
	length{ len } {
	if (len <= 0) {
		throw std::logic_error("error length");
	}
}

topit::p_t topit::VLine::begin() const {
	return segment;
}

topit::p_t topit::VLine::next(p_t prev) const {
	if (prev == segment) {
		if (length > 1) {
			return { segment.x, segment.y + 1 };
		}
		return segment;
	}
	int b = prev.y - segment.y;
	if (b < length - 1) {
		return { prev.x, prev.y + 1 };
	}
	return segment;
}
