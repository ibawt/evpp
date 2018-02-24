#include "sprite_sheet.hpp"
#include <array>
#include <fstream>
#include <sstream>

namespace ev {

SpriteSheet::SpriteSheet(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.good()) {
    throw std::runtime_error("file not exist");
  }
  json j = json::parse(file);

  auto metadata = j.at("metadata").get<std::unordered_map<std::string, json>>();
  textureSize = parse_size(metadata.at("size").get<std::string>());
  auto target =
      metadata.at("target").get<std::unordered_map<std::string, json>>();
  textureName =
      target.at("textureFileName").get<std::string>() +
      target.at("textureFileExtension").get<std::string>();

  auto json_frames =
      j.at("frames").get<std::unordered_map<std::string, json>>();

  for (const auto &frame : json_frames) {
    frames[frame.first] =
        std::make_shared<SpriteFrame>(frame.second, textureSize);
  }
}

Sprite SpriteSheet::create_sprite(
    std::initializer_list<const char *> frame_names) const {
  Sprite s;
  for (const auto &frame : frame_names) {
    s.animation.add_frame(frames.at(std::string(frame)));
  }
  return s;
}
}
