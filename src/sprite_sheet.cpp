#include "sprite_sheet.hpp"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <array>

using json = nlohmann::json;

namespace ev {

static std::vector<int32_t> parse_numbers(const std::string& s, unsigned n)
{
  std::istringstream is(s);
  std::vector<int32_t> numbers(n);

  while (!is.eof() && numbers.size() < n) {
    char c = is.peek();
    switch (c) {
    case EOF:
      break;
    case '}':
    case '{':
    case ' ':
    case ',':
      is.get();
      break;
    default:
      if (isdigit(c)) {
        int32_t i;
        is >> i;
        numbers.push_back(i);
        continue;
      }
      else {
        is.get();
      }
    }
  }
  if (numbers.size() != n) {
    throw std::runtime_error("invalid size!");
  }
  return numbers;
}

static Size parse_size(const std::string& s)
{
  auto n = parse_numbers(s, 2);
  return Size(static_cast<float>(n[0]), static_cast<float>(n[1]));
}

static Rectangle parse_rect(const std::string &s)
{
  auto n = parse_numbers(s, 4);

  return Rectangle(static_cast<float>(n[0]),
                   static_cast<float>(n[1]),
                   static_cast<float>(n[2]),
                   static_cast<float>(n[3]));
}

static std::string get(const std::string& s, const json& j)
{
  return j.at(s).get<std::string>();
}

static SpriteFrame parse_sprite_frame(const json& j)
{
  SpriteFrame s;

  s.trimmed = j.at("spriteTrimmed").get<bool>();
  s.rotated = j.at("textureRotated").get<bool>();

  s.source_size = parse_size(get("spriteSourceSize", j));
  s.size = parse_size(get("spriteSize", j));
  s.texture_rect = parse_rect(get("textureRect", j));
  s.color_rect = parse_rect(get("spriteColorRect", j));

  return s;
}

SpriteSheet::SpriteSheet(const std::string& filename)
{
  json j = json::parse(std::ifstream(filename));

  auto json_frames = j.at("frames").get<std::unordered_map<std::string, json>>();

  for (const auto& frame : json_frames) {
    frames[frame.first] = parse_sprite_frame(frame.second);
  }
}


}
