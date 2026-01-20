#include "geom.hpp"
#include <stdexcept>
#include <algorithm>

topit::f_t topit::frame(const p_t* pts, size_t s) {
	int minx = pts[0].x, miny = pts[0].y;
	int maxx = minx, maxy = miny;
	for (size_t i = 0; i < s; ++i) {
		minx = std::min(minx, pts[i].x); //pts[i].x < minx ? pts[i].x : minx;
		miny = std::min(miny, pts[i].y);
		maxx = std::max(maxx, pts[i].x);
		maxy = std::max(maxy, pts[i].y);
	}
	p_t a{ minx, miny };
	p_t b{ maxx, maxy };
	return f_t{ a, b };
}

bool topit::operator==(p_t a, p_t b) {
	return a.x == b.x && a.y == b.y;
}

bool topit::operator!=(p_t a, p_t b) {
	return !(a == b);
}

size_t topit::rows(f_t fr) {
	return (fr.bb.y - fr.aa.y + 1);
}

size_t topit::cols(f_t fr) {
	return (fr.bb.x - fr.aa.x + 1);
}