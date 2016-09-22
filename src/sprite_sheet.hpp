#pragma once

#include "ev.hpp"
#include <array>
#include <string>
#include <unordered_map>

namespace ev {

class SpriteFrame : Object
{
public:
  SpriteFrame() : trimmed(false), rotated(false) {}

  static const auto NUM_VERTS = 6;
  std::string key;
  Size source_size;
  Size size;
  bool trimmed;
  Rectangle texture_rect;
  Vec2 offset;
  bool rotated;
  Rectangle color_rect;
  std::array<BatchVertex, NUM_VERTS> batch_verts;
};

class SpriteSheet : Object
{
public:
  explicit SpriteSheet(const std::string& filename);
  SpriteSheet() { }

  SpriteFrame& get_frame(const std::string&s);
  const SpriteFrame& get_frame(const std::string& s) const;
private:
  std::unordered_map<std::string, SpriteFrame> frames;
};
}
