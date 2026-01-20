#include <iostream>
#include <stdexcept>
#include "ascii_draw.hpp"

int main() {
	using namespace topit;
	int err = 0;
	IDraw* shp[11] = {}; 
	p_t* pts = nullptr;
	size_t s = 0;
	try {
		shp[0] = new Diamond({15, 10}, 5);
		for (size_t i = 0; i < 1; ++i) {
			append(shp[i], &pts, s);
		}
		f_t fr = frame(pts, s);
		char* cnv = canvas(fr, '.');
		for (size_t i = 0; i < s; ++i) {
			paint(pts[i], cnv, fr, '#');
		}
		flush(std::cout, cnv, fr);
		delete [] cnv;
	} catch(...) {
		std::cerr << "Error!\n";
		err = 1;
	}
	delete shp[3];
	delete shp[2];
	delete shp[1];
	delete shp[0];
	return err;
}
