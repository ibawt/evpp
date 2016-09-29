#pragma once

#include "ev.hpp"
#include <array>
#include <string>
#include <memory>
#include <unordered_map>
#include "json.hpp"

namespace ev {

class SpriteFrame : Object
{
public:
  SpriteFrame() : trimmed(false), rotated(false) {}
  explicit SpriteFrame(const nlohmann::json&, const Size&);

  static const auto NUM_VERTS = 6;
  typedef std::array<BatchVertex, NUM_VERTS> BatchVertices;

  Size source_size;
  Size size;
  bool trimmed;
  Rectangle texture_rect;
  Vec2 offset;
  bool rotated;
  Rectangle color_rect;
  BatchVertices batch_verts;
};

std::ostream& operator<<(std::ostream& os, const SpriteFrame& s);

class SpriteSheet : Object
{
public:
  explicit SpriteSheet(const std::string& filename);
  SpriteSheet() { }

  std::shared_ptr<SpriteFrame>& operator[](const std::string& s) { return frames.at(s); }
  const std::shared_ptr<SpriteFrame>& operator[](const std::string& s) const { return frames.at(s); }

  const std::string& texture_filename() const { return textureName; }
private:
  std::string textureName;
  Size        textureSize;
  std::unordered_map<std::string, std::shared_ptr<SpriteFrame>> frames;
};
}
