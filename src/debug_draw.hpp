#pragma once
#include "ev.hpp"
#include "Box2D/Box2D.h"
#include "shader_program.hpp"
#include "vertex_buff.hpp"

namespace ev {

class b2DebugDraw : public b2Draw
{
 public:
    b2DebugDraw(float ratio);

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawString(int x, int y, const char* string, ...);
    void DrawAABB(b2AABB* aabb, const b2Color& color);
    void SetTransform(const mat4& );
    void DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count);
private:
  //TODO: these member names are inconsistent
  void SetColor(const b2Color& c, float ratio = 1.0f);
  mat4 mMatrix;
  ShaderProgram mShader;
  GLint mColorLocation;
  float mRatio;

  VertexBuff<float>     vbuff;
  VertexBuff<float>     segment_vbuff;
  VertexBuff<glm::vec4> color_buff;
  VertexBuff<int>       particle_shader;
};
}
