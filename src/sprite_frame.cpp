#include "sprite_frame.hpp"
namespace ev {
std::ostream &operator<<(std::ostream &os, const SpriteFrame &s) {
  os << "Source Size: " << s.source_size << std::endl
     << "Size: " << s.size << std::endl
     << "Trimmed: " << s.trimmed << std::endl
     << "Texture Rectangle: " << s.texture_rect << std::endl
     << "Offset: " << s.offset << std::endl
     << "Rotated: " << s.rotated << std::endl
     << "Color Rectangle: " << s.color_rect << std::endl;
  return os;
}
static std::vector<int32_t> parse_numbers(const std::string &s, unsigned n) {
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
      } else {
        is.get();
      }
    }
  }
  if (numbers.size() != n) {
    throw std::runtime_error("invalid size!");
  }
  return numbers;
}

Size parse_size(const std::string &s) {
  auto n = parse_numbers(s, 2);
  return Size(static_cast<float>(n[0]), static_cast<float>(n[1]));
}

Rectangle parse_rect(const std::string &s) {
  auto n = parse_numbers(s, 4);

  return Rectangle(static_cast<float>(n[0]), static_cast<float>(n[1]),
                   static_cast<float>(n[2]), static_cast<float>(n[3]));
}

static std::string get(const std::string &s, const json &j) {
  return j.at(s).get<std::string>();
}

static void fill_batch_verts(SpriteFrame &s) {
  SpriteFrame::BatchVertices &bv = s.batch_verts;

  bv[0].position.x = -s.size.width / 2;
  bv[0].position.y = -s.size.height / 2;
  bv[0].tex.x = s.texture_rect.origin.x;
  bv[0].tex.y = s.texture_rect.origin.y;

  bv[1].position.x = s.size.width / 2;
  bv[1].position.y = -s.size.height / 2;
  bv[1].tex.x = s.texture_rect.right();
  bv[1].tex.y = s.texture_rect.top();

  bv[2].position.x = s.size.width / 2;
  bv[2].position.y = s.size.height / 2;
  bv[2].tex.x = s.texture_rect.right();
  bv[2].tex.y = s.texture_rect.bottom();

  bv[3] = bv[2];

  bv[4].position.x = -s.size.width / 2;
  bv[4].position.y = s.size.height / 2;
  bv[4].tex.x = s.texture_rect.left();
  bv[4].tex.y = s.texture_rect.bottom();

  bv[5] = bv[0];
}

SpriteFrame::SpriteFrame(const json &j, const Size &textureSize) {
  trimmed = j.at("spriteTrimmed").get<bool>();
  try {
    rotated = j.at("textureRotated").get<bool>();
  } catch (std::exception &e) {
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

}
