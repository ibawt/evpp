#ifndef SPRITE_BATCH_H_
#define SPRITE_BATCH_H_

#include "ev.hpp"
#include "shader_program.hpp"
#include "sprite.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"
#include "vertex_buff.hpp"
#include <cassert>
#include <memory>
#include <vector>

namespace ev {

class SpriteBatch {
public:
  class Filler {
  public:
    explicit Filler(SpriteBatch &sb)
        : parent(sb), batch_verts(parent.vertex_buff.map()) {}
    ~Filler() { unmap(); }

    void render(const BatchVertexFiller &s) {
      assert(batch_verts != nullptr);
      assert(filled_vertices < parent.get_capacity() );
      filled_vertices += s.fill(batch_verts + filled_vertices);
    }

    void unmap() {
      if (batch_verts != nullptr) {
        parent.vertex_buff.unmap();
        batch_verts = nullptr;
      }
    }

    uint32_t num_filled_vertices() const { return filled_vertices; }

  private:
    Filler() = delete;
    Filler(const Filler &) = delete;
    Filler &operator=(const Filler &) = delete;

    SpriteBatch &parent;
    BatchVertex *batch_verts;
    uint32_t filled_vertices = 0;
  };

  SpriteBatch(const SpriteBatch& s);
  SpriteBatch(std::shared_ptr<Texture> texture);

  void set_capacity(size_t s) {
    vertex_buff.set_capacity(s);
  }

  size_t get_capacity() const { return vertex_buff.size(); }
  void set_texture(std::shared_ptr<Texture> t) { texture = t; }
  void render(Filler &filler, const glm::mat4 &);
  void set_blend_mode(BlendMode m) { blend_mode = m; }
  BlendMode &get_blend_mode() { return blend_mode; }
  friend class Filler;
private:
  BlendMode blend_mode = BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ShaderProgram program;
  std::shared_ptr<Texture> texture;
  VertexBuff<BatchVertex> vertex_buff;
};
}
#endif
