#pragma once

namespace ev {

struct Matrix4 {
  float m[16];

  void set_identity();
  void set_ortho(float l, float r, float bottom, float top, float near,
                 float far);
};
}
