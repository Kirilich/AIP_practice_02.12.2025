#ifndef DOT_HPP
#define DOT_HPP
#include "idraw.hpp"

namespace topit {
  struct Dot: IDraw {
		//IDraw имеет деструктор, поэтому здесь не реализовываем его
		explicit Dot(p_t dd); //запрещает неявное преобразование из p_t в Dot
		p_t begin() const override;
		p_t next(p_t prev) const override;
		p_t d;
	};
}

#endif
