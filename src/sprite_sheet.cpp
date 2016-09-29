#include "sprite_sheet.hpp"
#include <fstream>
#include <sstream>
#include <array>

using json = nlohmann::json;

namespace ev {

std::ostream& operator<<(std::ostream& os, const SpriteFrame& s)
{
  os << "Source Size: " << s.source_size << std::endl
     << "Size: " << s.size << std::endl
     << "Trimmed: " << s.trimmed << std::endl
     << "Texture Rectangle: " << s.texture_rect << std::endl
     << "Offset: " << s.offset << std::endl
     << "Rotated: " << s.rotated << std::endl
     << "Color Rectangle: " << s.color_rect << std::endl;
  return os;
}

static std::vector<int32_t> parse_numbers(const std::string& s, unsigned n)
{
  std::istringstream is(s);
  std::vector<int32_t> numbers;
  numbers.reserve(n);

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

static void fill_batch_verts(SpriteFrame& s)
{
  SpriteFrame::BatchVertices& bv = s.batch_verts;

  bv[0].x = -s.size.width/2;
  bv[0].y = -s.size.height/2;
  bv[0].u = s.texture_rect.origin.x;
  bv[0].v = s.texture_rect.origin.y;

  bv[1].x = s.size.width/2;
  bv[1].y = -s.size.height/2;
  bv[1].u = s.texture_rect.right();
  bv[1].v = s.texture_rect.top();

  bv[2].x = s.size.width/2;
  bv[2].y = s.size.height/2;
  bv[2].u = s.texture_rect.right();
  bv[2].v = s.texture_rect.bottom();

  bv[3] = bv[2];

  bv[4].x = -s.size.width/2;
  bv[4].y = s.size.height/2;
  bv[4].u = s.texture_rect.left();
  bv[4].v = s.texture_rect.bottom();

  bv[5] = bv[0];

  for(auto& x : s.batch_verts) {
    x.scale = 1.0f;
    x.rotation = 0.0f;
    x.tx = 0.0f;
    x.ty = 0.0f;
    x.opacity = 1.0f;
  }
}

SpriteFrame::SpriteFrame(const json& j, const Size& textureSize)
{
  trimmed = j.at("spriteTrimmed").get<bool>();
  try {
    rotated = j.at("textureRotated").get<bool>();
  } catch(std::exception & e) {
    rotated = false;
  }

  source_size = parse_size(get("spriteSourceSize", j));
  size = parse_size(get("spriteSize", j));
  texture_rect = parse_rect(get("textureRect", j));
  texture_rect.origin.x /= textureSize.width;
  texture_rect.origin.y /= textureSize.height;
  texture_rect.size.width /= textureSize.width;
  texture_rect.size.height /= textureSize.height;
  color_rect = parse_rect(get("spriteColorRect", j));
  offset = to_vec2(parse_size(get("spriteOffset", j)));

  fill_batch_verts(*this);
}

SpriteSheet::SpriteSheet(const std::string& filename)
{
  std::ifstream file(filename);
  if(! file.good() )  {
    throw std::runtime_error("file not exist");
  }
  json j = json::parse(file);

  auto metadata = j.at("metadata").get<std::unordered_map<std::string, json>>();
  textureSize = parse_size(get("size", metadata));
  auto target = metadata.at("target").get<std::unordered_map<std::string, json>>();
  textureName = get("textureFileName", target) + get("textureFileExtension", target);

  auto json_frames = j.at("frames").get<std::unordered_map<std::string, json>>();

  for (const auto& frame : json_frames) {
    frames[frame.first] = std::make_shared<SpriteFrame>(frame.second, textureSize);
  }

}


}
