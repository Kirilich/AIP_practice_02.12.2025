#ifndef DIAMOND_HPP
#define DIAMOND_HPP
#include "idraw.hpp"

namespace topit {
  struct Diamond : IDraw {
    explicit Diamond(p_t left_point, int side_length);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    p_t left_;
    int side_len_;
  };
}

#endif