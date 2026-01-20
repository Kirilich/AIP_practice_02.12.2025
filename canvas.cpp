#include "canvas.hpp"
#include <iostream>

void topit::flush(std::ostream& os, const char* cnv, f_t fr) {
	for (size_t i = 0; i < rows(fr); i++) {
		for (size_t j = 0; j < cols(fr); ++j) {
			os << cnv[i * cols(fr) + j];
		}
		os << "\n";
	}
}

void topit::paint(p_t p, char* cnv, f_t fr, char fill) {
	size_t dx = p.x - fr.aa.x;
	size_t dy = fr.bb.y - p.y;
	cnv[dy * cols(fr) + dx] = fill;
}

char* topit::canvas(f_t fr, char fill) {
	size_t s = rows(fr) * cols(fr);
	char* c = new char[s];
	for (size_t i = 0; i < s; ++i) {
		c[i] = fill;
	}
	return c;
}