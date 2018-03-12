#include "sprite_batch.hpp"
#include "gtc/type_ptr.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"

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

SpriteBatch::SpriteBatch(std::shared_ptr<Texture> tex)
    : program(animationVertexShader, defaultFragmentShader), texture(tex) {}

#define OFFSET_OF(x, y) (void *)(offsetof(x, y))

void SpriteBatch::render(const glm::mat4 &m) {
  vertex_buff.bind();
  glEnable(GL_TEXTURE_2D);
  texture->bind();
  program.use();

  glUniformMatrix4fv(program.get_uniform_loc("u_projTrans"), 1, GL_FALSE,
                     glm::value_ptr(m));

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
                        OFFSET_OF(BatchVertex, position));
  glVertexAttribPointer(tex, 2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, tex));
  glVertexAttribPointer(transform, 2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, rotation));
  glVertexAttribPointer(translation, 2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, translation));
  glVertexAttribPointer(opacity, 1, GL_FLOAT, GL_TRUE, sizeof(BatchVertex),
                        OFFSET_OF(BatchVertex, opacity));

  glEnable(GL_BLEND);
  glBlendFunc(blend_mode.src, blend_mode.dst);

  glDrawArrays(GL_TRIANGLES, 0, num_verts);
}
}
