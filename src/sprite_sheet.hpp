#pragma once

#include "ev.hpp"
#include <array>
#include <string>
#include <unordered_map>

namespace ev {

class SpriteFrame : Object
{
 public:
  SpriteFrame();

 private:
  std::string key;
  Size source_size;
  Size size;
  bool trimmed;
  Rectangle texture_rect;
  Vec2 offset;
  bool rotated;
  Rectangle color_rect;
  std::array<BatchVertex,6> batch_verts;
};

class SpriteSheet : Object
{
 public:
  SpriteSheet(const std::string& filename);
  SpriteSheet();

  SpriteFrame& GetFrame(const std::string&s);
  const SpriteFrame& GetFrame(const std::string& s) const;
 private:
  std::unordered_map<std::string, SpriteFrame> frames;
};
}
