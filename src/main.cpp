#include "application.hpp"
#include "ev.hpp"
#include "matrix4.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"
#include <iostream>
#include <memory>

#define STRINGIFY(x) #x

static std::string filepath(const std::string& filename)
{
	static const std::string source_directory = STRINGIFY(SOURCE_DIRECTORY);
	return std::string(STRINGIFY(SOURCE_DIRECTORY)) + "/" + filename;
}

class TestBed : public ev::Application {
public:
  TestBed()
      : ev::Application(800, 600, "ev"),
        tex(std::make_shared<ev::Texture>(filepath("flappybirds.png"))),
        sheet(std::make_shared<ev::SpriteSheet>(filepath("flappybird.json"))),
        batch(sheet, tex) {
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 800, 600);

    transform.set_ortho(0, 800, 600, 0, 1, -1);

    auto s = batch.create_sprite({"blue_bird_1"});
    s->position.x = 400;
    s->position.y = 300;
  }

  virtual void render() override {
    glClear(GL_COLOR_BUFFER_BIT);
    batch.render(transform);
  }

  virtual void update(const float dt) override { batch.update(dt); }

private:
  ev::Matrix4 transform;
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
