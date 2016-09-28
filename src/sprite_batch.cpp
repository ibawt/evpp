#include "sprite_batch.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"
#include "matrix4.hpp"

#include <algorithm>

static const auto animationVertexShader =
    "#version 120\n"
    "attribute vec2 a_position;\n"
    "attribute vec2 a_texCoord0;\n"
    "attribute vec2 transform;\n"
    "attribute vec2 translation;\n"
    "attribute float opacity;\n"
    "varying float out_opacity;\n"
    "uniform mat4 u_projTrans;\n"
    "varying vec2 v_texCoords;\n"
    "void main()\n"
    "{\n"
    "mat4 t = mat4( cos( transform.x ),      -sin( transform.x ), 0.0, 0.0,\n"
    "                    sin( transform.x ),  cos( transform.x ), 0.0, 0.0,\n"
    "                    0.0,                 0.0,                1.0, 0.0,\n"
    "                    translation.x,       translation.y,      0.0, 1.0 );\n"
    "v_texCoords = a_texCoord0;\n"
    "out_opacity = opacity;\n"
    "gl_Position =  u_projTrans * t * vec4(transform.y * a_position.x, "
    "transform.y *a_position.y,0.0,1.0) ;\n"
    "}\n";

static const auto defaultFragmentShader =
    "#version 120\n"
    "varying vec2 v_texCoords;\n"
    "varying float out_opacity;\n"
    "uniform sampler2D u_texture;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = texture2D(u_texture, v_texCoords) * "
    "vec4(1,1,1,out_opacity) ;\n"
    "}\n";

namespace ev {

SpriteBatch::SpriteBatch(std::shared_ptr<SpriteSheet> sheet)
    : blend_mode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), filled_vertex_count(0),
      program(animationVertexShader, defaultFragmentShader), sheet(sheet) {}

SpriteBatch::SpriteBatch(std::shared_ptr<SpriteSheet> sheet,
                         std::shared_ptr<Texture> tex)
    : blend_mode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), filled_vertex_count(0),
      program(animationVertexShader, defaultFragmentShader), texture(tex),
      sheet(sheet) {}

std::shared_ptr<Sprite>
SpriteBatch::create_sprite(std::initializer_list<const char *> frames) {
  auto s = std::make_shared<Sprite>();

  for (auto frame : frames) {
    s->animation.add_frame((*sheet)[std::string(frame)]);
  }

  sprites.push_back(s);
  return sprites.back();
}

void SpriteBatch::remove(const std::shared_ptr<Sprite>& s)
{
  auto i = std::find(sprites.begin(), sprites.end(), s);

  sprites.erase(i);
}

void SpriteBatch::update(float dt) {
  auto b = vertex_buff.map();
  filled_vertex_count = 0;

  for (auto &s : sprites) {
    s->update(dt);

    filled_vertex_count += s->fill(b + filled_vertex_count);
  }

  vertex_buff.unmap();
}

#define OFFSET_OF(x, y) (void *)(offsetof(x, y))

void SpriteBatch::render(const Matrix4 &m) {
  vertex_buff.bind();
  glEnable(GL_TEXTURE_2D);
  texture->bind();
  program.use();

  glUniformMatrix4fv(program.get_uniform_loc("u_projTrans"), 1, GL_FALSE, m.m);

  int pos = program.get_attrib_loc("a_position");
  int tex = program.get_attrib_loc("a_texCoord0");
  int transform = program.get_attrib_loc("transform");
  int translation = program.get_attrib_loc("translation");
  int opacity = program.get_attrib_loc("opacity");

  glEnableVertexAttribArray(pos);
  glEnableVertexAttribArray(tex);
  glEnableVertexAttribArray(transform);
  glEnableVertexAttribArray(translation);
  glEnableVertexAttribArray(opacity);

  glVertexAttribPointer(pos, 2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, x));
  glVertexAttribPointer(tex, 2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, u));
  glVertexAttribPointer(transform, 2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, rotation));
  glVertexAttribPointer(translation, 2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, tx));
  glVertexAttribPointer(opacity, 1, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, opacity));

  glEnable(GL_BLEND);
  glBlendFunc(blend_mode.src, blend_mode.dst);

  glDrawArrays(GL_TRIANGLES, 0, filled_vertex_count);
}
}
