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
	struct VLine : IDraw {
		VLine(p_t id, size_t len);
		p_t begin() const override;
		p_t next(p_t prev) const override;
		p_t segment;
		size_t length;
	};
	struct Rect : IDraw {
		Rect(p_t pos, int w, int h);
		Rect(p_t a, p_t b);
		p_t begin() const override;
		p_t next(p_t prev) const override;
		f_t rect;
	};
	struct FRect : IDraw {
		FRect(p_t pos, int w, int h);
		FRect(p_t a, p_t b);
		p_t begin() const override;
		p_t next(p_t prev) const override;
		f_t rect;
	};
	p_t* extend(const p_t* pts, size_t s, p_t fill);
	void extend(p_t** pts, size_t& s, p_t fill);
	void append(const IDraw* sh, p_t** ppts, size_t& s);
	f_t frame(const p_t* pts, size_t s);
	struct Layers;
	f_t frame(const Layers& ls);
	char* canvas(f_t fr, char fill);
	void paint(p_t p, char* cnv, f_t fr, char fill);
	void flush(std::ostream& os, const char* cnv, f_t fr);
	struct Layers {
		Layers();
		~Layers();
		Layers(const& Layers) = delete; //hw
		Layers& operator=(const& Layers) = delete; //homework
		Layers(Layers&&) = delete; //hw
		Layers& operator=(Layers&&) = delete; //homework
		void append(const IDraw& dr);
		size_t points() const {
			return points_;
		}
		size_t layers() const {
			return layers_;
		}
		size_t start(size_t i) const {
			return !i ? 0 : sizes_[i - 1];
		}
		size_t end(size_t i) const {
			return sizes_[i];
		}
		p_t  point(size_t i) const {
			return pts_[i];
		}
	private:
		size_t points_;
		p_t * pts_;
		size_t layers_;
		size_t* sizes_;
	};
}

int main() {
	using namespace topit;
	int err = 0;

	IDraw* shp[8] = {}; 
	Layers layers;
	try {
		shp[0] = new FRect({-3, -2}, {3, 3}); 
		shp[1] = new Dot({ -3, 3 });
		shp[2] = new Dot({ -2, 4 });
		shp[3] = new Dot({ -1, 5 });
		shp[4] = new Dot({ 0, 6 });
		shp[5] = new Dot({ 1, 5 });
		shp[6] = new Dot({ 2, 4 });
		shp[7] = new Dot({ 3, 3 });
		for (size_t i = 0; i < 8; ++i) {
			layers.append(*(shp[i]));
		}
		f_t fr = frame(layers);
		char* cnv = canvas(fr, '.');
		const char * brush = "#0@%$+=7";
		for (size_t k = 0; k < layers.layers(); ++k) {
			size_t start = layers.start(k);
			size_t end = layers.end(k);
			for (size_t i = start; i < end; ++i) {
				paint(layers.point(i), cnv, fr, brush[k]);
			}
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

void topit::Layers::append(const IDraw& dr) {
	size_t* ext_sizes = new size_t[layers_ + 1];
	try {
		topit::append(&dr, &pts_, points_);
	}
	catch (...) {
		delete[] ext_sizes;
		throw;
	}
	for (size_t i = 0; i < layers_; ++i) {
		ext_sizes[i] = sizes_[i];
	}
	ext_sizes[layers_] = points_;
	delete[] sizes_;
	sizes_ = ext_sizes;
	++layers_;
}

topit::Layers::Layers() :
	points_{0},
	pts_{nullptr},
	layers_{0},
	sizes_{nullptr}
{}

topit::Layers::~Layers() {
	delete[] pts_;
	delete[] sizes_;
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

topit::VLine::VLine(p_t id, size_t len) :
	IDraw(),
	segment{ id },
	length{ len } {
	if (len == 0) {
		throw std::logic_error("zero length");
	}
}

topit::p_t topit::VLine::begin() const {
	return segment;
}

topit::p_t topit::VLine::next(p_t prev) const {
	if (prev == segment) {
		if (length > 1) {
			return { segment.x, segment.y + 1};
		}
		return segment;
	}
	int b = prev.y - segment.y;
	if (b < length - 1) {
		return { prev.x, prev.y + 1 };
	}
	return segment;
}

topit::Rect::Rect(p_t pos, int w, int h) :
	IDraw(),
	rect{pos, {pos.x + w, pos.y + h}}
{
	if (!(w > 0 && h > 0)) {
		throw std::logic_error("bad rect");
	}
}

topit::Rect::Rect(p_t a, p_t b) :
	Rect(a, b.x - a.x, b.y - a.y)
{}

topit::p_t topit::Rect::begin() const {
	return rect.aa;
}

topit::p_t topit::Rect::next(p_t prev) const {
	if (prev.x == rect.aa.x && prev.y < rect.bb.y) {
		return { prev.x, prev.y + 1 };
	}
	else if (prev.y == rect.bb.y && prev.x < rect.bb.x) {
		return { prev.x + 1, prev.y };
	}
	else if (prev.x == rect.bb.x && prev.y > rect.aa.y) {
		return { prev.x, prev.y - 1 };
	}
	else if (prev.y == rect.aa.y && prev.x > rect.aa.x) {
		return { prev.x - 1, prev.y };
	}
	throw std::logic_error("bad impl");
}

topit::FRect::FRect(p_t pos, int w, int h) :
	IDraw(),
	rect{ pos, {pos.x + w, pos.y + h} }
{
	if (!(w > 0 && h > 0)) {
		throw std::logic_error("bad rect");
	}
}

topit::FRect::FRect(p_t a, p_t b) :
	FRect(a, b.x - a.x, b.y - a.y)
{
}

topit::p_t topit::FRect::begin() const {
	return rect.aa;
}

topit::p_t topit::FRect::next(p_t prev) const {
	if (prev.x < rect.bb.x) {
		return { prev.x + 1, prev.y };
	}
	else if (prev.x == rect.bb.x && prev.y < rect.bb.y) {
		return { rect.aa.x, prev.y + 1 }; //jump on begin
	}
	else if (prev == rect.bb) {
		return rect.aa;
	}
	throw std::logic_error("bad impl");
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