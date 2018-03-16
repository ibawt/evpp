#ifndef SPRITE_BATCH_H_
#define SPRITE_BATCH_H_

#include "ev.hpp"
#include "shader_program.hpp"
#include "sprite.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"
#include "vertex_buff.hpp"
#include <memory>
#include <vector>

namespace ev {

class SpriteBatch {
public:
  SpriteBatch(std::shared_ptr<Texture> texture);

  void set_capacity(size_t s) {
    vertex_buff.set_capacity(s);
  }

  template <typename FillFunction>
  void fill(FillFunction&& fn) {
    num_verts = 0;
    auto bv = vertex_buff.mapped_buffer();
    num_verts += fn( &bv[num_verts]);
  }

  size_t get_capacity() const { return vertex_buff.size(); }
  void set_texture(std::shared_ptr<Texture> t) { texture = t; }
  void render(const glm::mat4 &);
  void set_blend_mode(BlendMode m) { blend_mode = m; }
  BlendMode get_blend_mode() { return blend_mode; }
private:
  BlendMode blend_mode = BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ShaderProgram program;
  std::shared_ptr<Texture> texture;
  VertexBuff<BatchVertex> vertex_buff;

  int num_verts = 0;
};
}
#endif
