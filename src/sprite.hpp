#ifndef SPRITE_H_
#define SPRITE_H_

#include "Box2D/Box2D.h"
#include "ev.hpp"
#include "sprite_sheet.hpp"
#include <memory>
#include <vector>

namespace ev {

class Animation : public Object {
public:
  enum class Mode { LOOP, ONE_SHOT, REVERSE, PING_PONG };

  Animation() : delay(0.3f), time(0.0f), mode(Mode::LOOP), index(0) {}

  void update(float dt);
  void add_frame(std::shared_ptr<SpriteFrame> frame) {
    frames.push_back(frame);
  }

  void set_mode(Mode m) { mode = m; }
  Mode get_mode() const { return mode; }

  const std::shared_ptr<SpriteFrame> &current_frame() const {
    return frames[index];
  }

private:
  float delay;
  float time;
  Mode mode;
  uint32_t index;
  std::vector<std::shared_ptr<SpriteFrame>> frames;
};

class Sprite : public Object {
public:
  Sprite() : rotation(0.0f), scale(1.0f), visible(true), opacity(1.0f) {}

  uint32_t fill(BatchVertex *array) const {
    if (!visible) {
      return 0;
    }

    auto &frame = animation.current_frame()->batch_verts;

    for (int i = 0; i < 6; ++i) {
      array[i] = frame[i];
      array[i].scale = scale;
      array[i].rotation = rotation;
      array[i].tx = position.x;
      array[i].ty = position.y;
      array[i].opacity = opacity;
    }
    return 6;
  }

  virtual void update(float dt) { animation.update(dt); }

  virtual void set_position(float x, float y) {
    position.x = x;
    position.y = y;
  }

  Animation animation;
  Vec2 position;
  float rotation;
  float scale;
  bool visible;
  float opacity;
};

class PhysicsSprite : public Sprite {
public:
  explicit PhysicsSprite(b2Body *p, float ptm_ratio = 32.0f)
      : ptm_ratio(ptm_ratio), body(p) {}

  virtual void update(float dt) override {
    Sprite::update(dt);

    b2Vec2 p = body->GetPosition();

    position.x = p.x * ptm_ratio;
    position.y = p.y * ptm_ratio;

    rotation = body->GetAngle();
  }

  virtual void set_position(float x, float y) override {
    b2Vec2 v(x / ptm_ratio, y / ptm_ratio);

    body->SetTransform(v, rotation);
  }

private:
  const float ptm_ratio;
  b2Body *body;
};
}
#endif
