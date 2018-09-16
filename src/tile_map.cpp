#include "tile_map.hpp"
#include <algorithm>

namespace ev {
  int TileMap::fill(ev::BatchVertex *bv, const Tile& t, const vec2& position) {
      const float scale = 2.0f;

      const float tex_y_scale = texture_size.height;
      const float tex_x_scale = texture_size.width;

      auto tile_stride = texture_size.width / tile_size;

      auto ts_row = t.tile_set / 3;
      auto ts_col = t.tile_set % 3;

      int row = t.type / 6 + 3;
      int col = t.type % 6;

      const float tex_row = row * tile_size;
      const float tex_col = col * tile_size;

      // Upper left
      bv[0].position.x = -tile_size/2.0f;
      bv[0].position.y = -tile_size/2.0f;
      bv[0].opacity = 1.0f;
      bv[0].scale = scale;
      bv[0].translation = position;
      bv[0].tex.x = tex_col / tex_x_scale;
      bv[0].tex.y = tex_row / tex_y_scale;
      bv[0].rotation = 0.0f;

      // Upper right
      bv[1].position.x = tile_size/2.0f;
      bv[1].position.y = -tile_size/2.0f;
      bv[1].opacity = 1.0f;
      bv[1].scale = scale;
      bv[1].translation = position;
      bv[1].tex.x = (tex_col + tile_size) / tex_x_scale;
      bv[1].tex.y = tex_row / tex_y_scale;
      bv[1].rotation = 0.0f;

      // Lower Right
      bv[2].position.x = tile_size/2.0f;
      bv[2].position.y = tile_size/2.0f;
      bv[2].opacity = 1.0f;
      bv[2].scale = scale;
      bv[2].translation = position;
      bv[2].tex.x = (tex_col + tile_size) / tex_x_scale;
      bv[2].tex.y = (tex_row + tile_size) / tex_y_scale;
      bv[2].rotation = 0.0f;

      bv[3] = bv[2];

      // Lower Left
      bv[4].position.x = -tile_size/2.0f;
      bv[4].position.y = tile_size/2.0f;
      bv[4].opacity = 1.0f;
      bv[4].scale = scale;
      bv[4].translation = position;
      bv[4].tex.x = tex_col / tex_x_scale;
      bv[4].tex.y = (tex_row + tile_size) / tex_y_scale;
      bv[4].rotation = 0.0f;

      bv[5] = bv[0];

      return 6;
    }
  TileMap::TileMap(int rows, int cols, int tile_size, std::shared_ptr<Texture> tex) :
    rows(rows), columns(cols), tile_size(tile_size),texture_size(tex->get_size()), batch(tex)
  {
    tiles.resize(rows*cols);
    batch.set_capacity(rows*cols*6);
  }

  void TileMap::set_viewport(const Rectangle& r) {
    if( r.size.width < 0 || r.size.height < 0 ) {
      throw EVException("invalid rectangle");
    }
    view_port = r;
    view_port.origin.x = std::min(0.0f, r.origin.x);
    view_port.origin.y = std::min(0.0f, r.origin.y);
  }

  void TileMap::render(const mat4& transform)
  {
    batch.fill([this] (BatchVertex *bv)  {
        int numRows = std::max(static_cast<int>(view_port.size.height / tile_size), rows);
        int numCols = std::max(static_cast<int>(view_port.size.width / tile_size), columns);
        int num_filled = 0;

        for (int row = 0 ; row < numRows ; ++row ) {
          for(int col = 0 ; col < numCols ; ++col) {
            // const auto& tile = tiles[row * columns + col];
            const vec2 pos(col * tile_size, row * tile_size);
            num_filled += fill(bv + num_filled, tiles[row*columns + col], pos);
          }
        }
        return num_filled;
      });
    batch.render(transform);
  }

  TileMap::~TileMap() {
  }
}
