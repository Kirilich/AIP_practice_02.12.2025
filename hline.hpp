#ifndef HLINE_HPP
#define HLINE_HPP
#include "idraw.hpp"

namespace topit {
	struct HLine : IDraw {
		HLine(p_t id, size_t len);
		p_t begin() const override;
		p_t next(p_t prev) const override;
		p_t segment;
		size_t length;
	};
}

#endif