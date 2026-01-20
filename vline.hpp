#ifndef VLINE_HPP
#define VLINE_HPP
#include "idraw.hpp"

namespace topit {
	struct VLine : IDraw {
		VLine(p_t id, size_t len);
		p_t begin() const override;
		p_t next(p_t prev) const override;
		p_t segment;
		size_t length;
	};
}

#endif