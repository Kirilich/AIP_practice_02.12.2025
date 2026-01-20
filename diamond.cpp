#include "diamond.hpp"
#include <stdexcept>

topit::Diamond::Diamond(p_t left_point, int side_length):
  left_{ left_point },
  side_len_{ side_length } {
  if (side_length < 2) {
    throw std::logic_error("want side more 2");
  }
}

topit::p_t topit::Diamond::begin() const {
  return { left_.x + side_len_ - 1, left_.y };
}

topit::p_t topit::Diamond::next(p_t prev) const {
  int cx = left_.x + side_len_ - 1;
  int top_y = left_.y;
  int bottom_y = left_.y + 2 * (side_len_ - 1);
  static int state = 0;
  if (prev.x == cx && prev.y == top_y) {
    state = 0;
    return { cx + 1, top_y + 1 };
  }
  switch (state) {
  case 0:
    if (prev.x < cx + (side_len_ - 1)) {
      return { prev.x + 1, prev.y + 1 };
    }
    else {
      state = 1;
      return { prev.x - 1, prev.y + 1 };
    }
  case 1:
    if (prev.y < bottom_y) {
      return { prev.x - 1, prev.y + 1 };
    }
    else {
      state = 2;
      return { prev.x - 1, prev.y - 1 };
    }
  case 2:
    if (prev.x > cx - (side_len_ - 1)) {
      return { prev.x - 1, prev.y - 1 };
    }
    else {
      state = 3;
      return { prev.x + 1, prev.y - 1 };
    }
  case 3:
    if (prev.y > top_y) {
      return { prev.x + 1, prev.y - 1 };
    }
    else {
      return begin();
    }
  }
  throw std::logic_error("diamond error");
}