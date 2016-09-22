#include "sprite_sheet.hpp"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <array>

using json = nlohmann::json;

namespace ev {

static Size parse_size(const std::string& s)
{
	std::istringstream is(s);
	int32_t numbers[2];
	int pos = 0;

	while (!is.eof() && pos < 2) {
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
				is >> numbers[pos++];
				continue;
			}
			else {
				is.get();
			}
		}
	}
	if (pos != 2) {
		throw std::runtime_error("invalid size!");
	}
	return Size(static_cast<float>(numbers[0]), static_cast<float>(numbers[1]));
}

static SpriteFrame parse_sprite_frame(const json& j)
{
	SpriteFrame s;

	s.trimmed = j.at("spriteTrimmed").get<bool>();
	s.rotated = j.at("textureRotated").get<bool>();

	s.source_size = parse_size(j.at("spriteSourceSize").get<std::string>());

	return s;
}

SpriteSheet::SpriteSheet(const std::string& filename)
{
	std::ifstream in(filename);
	json j(in);
 
	auto json_frames = j.at("frames").get<std::unordered_map<std::string, json>>();

  for (const auto& frame : json_frames) {
	  frames[frame.first] = parse_sprite_frame(frame.second);
  }
}


}
