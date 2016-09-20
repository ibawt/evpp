#include "sprite_sheet.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

namespace ev {

SpriteSheet::SpriteSheet(const std::string& filename)
{
  std::ifstream in(filename);
  json j;

  in >> j;
}
}
