#include <iostream>
#include <stdexcept>
namespace topit {
	struct p_t { //point_t
		int x, y;
	};
	struct f_t { //frame_t
		p_t aa, bb;
	};
	size_t rows(f_t fr);
	size_t cols(f_t fr);
	bool operator==(p_t a, p_t b);
	bool operator!=(p_t a, p_t b);
	struct IDraw {
		virtual ~IDraw() = default;
		virtual p_t begin() const = 0;
		virtual p_t next(p_t prev) const = 0;
	};
	struct Dot: IDraw {
		//IDraw имеет деструктор, поэтому здесь не реализовываем его
		explicit Dot(p_t dd); //запрещает неявное преобразование из p_t в Dot
		p_t begin() const override;
		p_t next(p_t prev) const override;
		p_t d;
	};
	struct HLine: IDraw {
		HLine(p_t id, size_t len);
		p_t begin() const override;
		p_t next(p_t prev) const override;
		p_t segment;
		size_t length;
	};
	p_t* extend(const p_t* pts, size_t s, p_t fill);
	void extend(p_t** pts, size_t& s, p_t fill);
	void append(const IDraw* sh, p_t** ppts, size_t& s);
	f_t frame(const p_t* pts, size_t s);
	char* canvas(f_t fr, char fill);
	void paint(p_t p, char* cnv, f_t fr, char fill);
	void flush(std::ostream& os, const char* cnv, f_t fr);
}

int main() {
	using namespace topit;
	int err = 0;
	IDraw* shp[4] = {}; 
	p_t* pts = nullptr;
	size_t s = 0;
	try {
		shp[0] = new Dot({ 0, 0 });
		shp[1] = new Dot({ 2, 3 });
		shp[2] = new Dot({ -5, -2 });
		shp[3] = new HLine({ -2, -2 }, 4);
		for (size_t i = 0; i < 4; ++i) {
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

topit::p_t* topit::extend(const p_t* pts, size_t s, p_t fill) {
	p_t* r = new p_t[s + 1];
	for (size_t i = 0; i < s; ++i) {
		r[i] = pts[i];
	}
	r[s] = fill;
	return r;
}

void topit::extend(p_t** pts, size_t& s, p_t fill) {
	p_t* r = extend(*pts, s, fill);
	delete[] *pts;
	++s;
	*pts = r;
}


void topit::append(const IDraw* sh, p_t** ppts, size_t& s) {
	extend(ppts, s, sh->begin());//закнуть начало в массив
	p_t b = sh->begin();
	while (sh->next(b) != sh->begin()) {
		b = sh->next(b);
		extend(ppts, s, b);
	}
}

void topit::paint(p_t p, char* cnv, f_t fr, char fill) {
	size_t dx = p.x - fr.aa.x;
	size_t dy = fr.bb.y - p.y;
	cnv[dy * cols(fr) + dx] = fill;
}

void topit::flush(std::ostream& os, const char* cnv, f_t fr) {
	for (size_t i = 0; i < rows(fr); i++) {
		for (size_t j = 0; j < cols(fr); ++j) {
			os << cnv[i * cols(fr) + j];
		}
		os << "\n";
	}
}

char* topit::canvas(f_t fr, char fill) {
	size_t s = rows(fr) * cols(fr);
	char* c = new char[s];
	for (size_t i = 0; i < s; ++i) {
		c[i] = fill;
	}
	return c;
}

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

topit::Dot::Dot(p_t dd):
	IDraw(),
	d{dd} {	
}

topit::p_t topit::Dot::begin() const {
	return d;
}

topit::p_t topit::Dot::next(p_t prev) const {
	if (prev != d) {
		throw std::logic_error("bad prev");
	}
	return d;
}

topit::HLine::HLine(p_t id, size_t len) :
	IDraw(),
	segment{ id },
	length{ len } {
	if (len == 0) {
		throw std::logic_error("zero length");
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