#include "sprite.hpp"

namespace ev {

void Animation::update(float dt) {
  if (delay > 0.0f) {
    time += dt;

    while (time >= delay) {
      time -= delay;

      switch (mode) {
      case Mode::LOOP:
        index++;
        if (index >= static_cast<int>(frames.size()))
          index = 0;
        break;
      case Mode::ONE_SHOT:
        index++;
        if (index >= static_cast<int>(frames.size()))
          index = frames.size() - 1;
        break;
      case Mode::REVERSE:
        if (index > 0) {
          index--;
        }
        break;
      case Mode::PING_PONG:
        break;
      }
    }
  }
}
}
