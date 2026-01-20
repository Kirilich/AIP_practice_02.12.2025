#include "hline.hpp"
#include <stdexcept>

topit::HLine::HLine(p_t id, size_t len) :
	IDraw(),
	segment{ id },
	length{ len } {
	if (len <= 0) {
		throw std::logic_error("error length");
	}
}

topit::p_t topit::HLine::begin() const {
	return segment;
}

topit::p_t topit::HLine::next(p_t prev) const {
	if (prev == segment) {
		if (length > 1) {
			return { segment.x + 1, segment.y };
		}
		return segment;
	}
	int b = prev.x - segment.x;
	if (b < length - 1) {
		return { prev.x + 1, prev.y };
	}
	return segment;
}