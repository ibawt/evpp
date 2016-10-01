#include "application.hpp"
#include "ev.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"
#include <iostream>
#include <memory>
#include "gtc/matrix_transform.hpp"
#include "Box2D/Box2D.h"

#define xstr(x) #x
#define STRINGIFY(x) xstr(x)

static std::string filepath(const std::string& filename)
{
  static const std::string source_directory = STRINGIFY(SOURCE_DIRECTORY);
  return source_directory + "/test/" + filename;
}

class TestBed : public ev::Application {
public:
  TestBed()
      : ev::Application(800, 600, "ev"),
      world(b2Vec2(0.0f, 9.0f)),
      transform(glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 1.0f, -1.0f)),
      tex(std::make_shared<ev::Texture>(filepath("flappybirds.png"))),
      sheet(std::make_shared<ev::SpriteSheet>(filepath("flappybird.json"))),
      batch(sheet, tex) {
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, get_width(), get_height());

    auto s = batch.create_sprite({"blue_bird_1", "blue_bird_2", "blue_bird_3"});
    s->position.x = 400;
    s->position.y = 300;
    s->scale = 5.0f;

    auto s2 = batch.create_sprite({"background1"});

    auto s3 = batch.create_sprite({"green_pipe_down"});

    world.SetAllowSleeping(true);
    world.SetContinuousPhysics(true);
  }

  virtual void render() override {
    glClear(GL_COLOR_BUFFER_BIT);
    batch.render(transform);
  }

  virtual void update(const float dt) override {
    // printf("%f\n", dt);
    batch.update(dt);
    world.Step(dt, 8, 3);
  }

private:
  b2World     world;
  glm::mat4   transform;
  std::shared_ptr<ev::Texture> tex;
  std::shared_ptr<ev::SpriteSheet> sheet;
  ev::SpriteBatch batch;
};

int main(int argc, char **argv) {
  try {
    TestBed t;

    t.show();
    t.run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
