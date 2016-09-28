#ifndef SPRITE_BATCH_H_
#define SPRITE_BATCH_H_

#include "ev.hpp"
#include "shader_program.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"
#include "sprite.hpp"
#include "matrix4.hpp"
#include "vertex_buff.hpp"

#include <memory>
#include <vector>

namespace ev {

class SpriteBatch : Object {
public:
  explicit SpriteBatch(std::shared_ptr<SpriteSheet> sheet);
  explicit SpriteBatch(std::shared_ptr<SpriteSheet> sheet,
                       std::shared_ptr<Texture> texture);
  std::shared_ptr<Sprite> create_sprite(std::initializer_list<const char *>);

  void set_texture(std::shared_ptr<Texture> t) { texture = t; }
  void update(float dt);
  void render(const Matrix4 &);
  void set_blend_mode(BlendMode m) { blend_mode = m; }
  BlendMode &get_blend_mode() { return blend_mode; }

private:
  BlendMode blend_mode;
  unsigned filled_vertex_count;
  ShaderProgram program;
  std::shared_ptr<Texture> texture;
  std::vector<std::shared_ptr<Sprite>> sprites;
  VertexBuff<BatchVertex> vertex_buff;
  std::shared_ptr<SpriteSheet> sheet;
};
}
#endif
