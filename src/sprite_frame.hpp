#pragma once

#include "ev.hpp"

namespace ev {
class SpriteFrame{
 public:
  SpriteFrame(const json &, const Size &);

  static const auto NUM_VERTS = 6;
  typedef std::array<BatchVertex, NUM_VERTS> BatchVertices;

  Size source_size;
  Size size;
  bool trimmed = false;
  Rectangle texture_rect;
  vec2 offset;
  bool rotated = false;
  Rectangle color_rect;
  BatchVertices batch_verts;
};

Size parse_size(const std::string&);
Rectangle parse_rect(const std::string&);

std::ostream &operator<<(std::ostream &os, const SpriteFrame &s);
}
