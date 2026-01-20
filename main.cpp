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
		shp[0] = new HLine({ -3, 2 }, 7);
		shp[1] = new VLine({ -4, -2 }, 5);
		shp[2] = new VLine({ 4, -2 }, 5);
		shp[3] = new HLine({ -3, -2 }, 7);  
		shp[4] = new Dot({ -3, 3 });
		shp[5] = new Dot({ -2, 4 });
		shp[6] = new Dot({ -1, 5 });
		shp[7] = new Dot({ 0, 6 });
		shp[8] = new Dot({ 1, 5 });
		shp[9] = new Dot({ 2, 4 });
		shp[10] = new Dot({ 3, 3 });
		for (size_t i = 0; i < 11; ++i) {
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
