#pragma once

#include "sprite_batch.hpp"

namespace ev
{
  enum TileType {
    OUTER_UPPER_LEFT = 0x0,
    OUTER_TOP = 0x1,
    OUTER_UPPER_RIGHT = 0x2,
    MIDDLE_SMALL = 0x3,
    TOP_T = 0x4,
    EMPTY_0 = 0x5,
    OUTER_LEFT = 0x6,
    MIDDLE_LARGE = 0x7,
    EMPTY_1 = 0x8,
    LEFT_T = 0x9,
    CROSS_T = 10,
    RIGHT_T = 11,
    OUTER_BOTTOM_LEFT = 12,
    EMPTY_2 = 13,
    OUTER_BOTTOM_RIGHT = 14,
    EMPTY_3 = 15,
    BOTTOM_T = 16,
    EMPTY_4 = 17
  };

  struct Tile {
    int tile_set = 0;
    TileType type = TileType::OUTER_UPPER_LEFT;
  };

  class TileMap {
  public:
    TileMap(int rows, int cols, int tile_size, std::shared_ptr<Texture> tex);
    virtual ~TileMap();

    void set_viewport(const Rectangle&);
    Rectangle get_viewport() const;

    int get_rows() const { return rows; }
    int get_cols() const { return columns; }
    int get_tile_size() const { return tile_size; }

    void render(const glm::mat4&);
  private:
    Rectangle view_port;
    int rows;
    int columns;
    int tile_size;

    std::vector<Tile> tiles;
    SpriteBatch batch;
  };
}
