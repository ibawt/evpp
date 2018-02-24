#pragma once

#include "ev.hpp"
#include "sprite.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace ev {
class SpriteSheet {
public:
  explicit SpriteSheet(const std::string &filename);

  const std::shared_ptr<SpriteFrame> &operator[](const std::string &s) const {
    return frames.at(s);
  }

  Sprite create_sprite(std::initializer_list<const char *>) const;

  const std::string &texture_filename() const { return textureName; }

private:
  std::string textureName;
  Size textureSize;
  std::unordered_map<std::string, std::shared_ptr<SpriteFrame>> frames;
};
}
