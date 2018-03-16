#ifndef SPRITE_H_
#define SPRITE_H_

#include "ev.hpp"
#include "sprite_frame.hpp"
#include <cassert>
#include <memory>
#include <vector>

namespace ev {

class Animation {
public:
  enum class Mode { LOOP, ONE_SHOT, REVERSE, PING_PONG };
  void update(float dt);
  void add_frame(std::shared_ptr<SpriteFrame> frame) {
    frames.push_back(frame);
  }

  void set_mode(Mode m) { mode = m; }
  Mode get_mode() const { return mode; }

  const std::shared_ptr<SpriteFrame> &current_frame() const {
    assert(frames.size());
    return frames[index];
  }

  friend std::ostream &operator<<(std::ostream &os, const Mode &m) {
    switch (m) {
    case Animation::Mode::LOOP:
      os << "LOOP";
      break;
    case Animation::Mode::ONE_SHOT:
      os << "ONE_SHOT";
      break;
    case Animation::Mode::REVERSE:
      os << "REVERSE";
      break;
    case Animation::Mode::PING_PONG:
      os << "PING PONG";
      break;
    }

    return os;
  }
  friend std::ostream &operator<<(std::ostream &os, const Animation &anim) {
    os << "Animation: delay: " << anim.delay << std::endl
       << "time: " << anim.time << std::endl
       << "mode: " << anim.mode << std::endl
       << "index: " << anim.index << std::endl;

    return os;
  }
  void  set_delay(float f) { delay = f; }
  float get_delay() const { return delay; }

  int get_index() const { return index; }
  size_t get_num_frames() const { return frames.size(); }
private:
  float delay = 0.3f;
  float time = 0.0f;
  Mode  mode = Mode::LOOP;
  int   index = 0;
  std::vector<std::shared_ptr<SpriteFrame>> frames;
};

  class Sprite {
  public:
  int fill(BatchVertex *array) const {
    if (!visible) {
      return 0;
    }

    auto &frame = animation.current_frame()->batch_verts;

    for (int i = 0; i < SpriteFrame::NUM_VERTS; ++i) {
      array[i] = frame[i];
      array[i].scale = scale;
      array[i].rotation = rotation;
      array[i].translation = position;
      array[i].opacity = opacity;
    }
    return 6;
  }

  void update(float dt) { animation.update(dt); }

  void set_position(float x, float y) {
    position.x = x;
    position.y = y;
  }

  void set_position(const vec2& v) {
    position = v;
  }

  Animation animation;
  vec2 position;
  float rotation = 0.0f;
  float scale = 1.0f;
  bool visible = true;
  float opacity = 1.0f;
};

inline std::ostream &operator<<(std::ostream &os, const Sprite &s) {
  os << "animation: " << s.animation << "position: " << s.position << std::endl
     << "rotation: " << s.rotation << std::endl
     << "scale: " << s.scale << std::endl
     << "visible: " << s.visible << std::endl
     << "opacity: " << s.opacity << std::endl;
  return os;
}
}
#endif
