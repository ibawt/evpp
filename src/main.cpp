#include "Box2D/Box2D.h"
#include "application.hpp"
#include "ev.hpp"
#include "gtc/matrix_transform.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"
#include "tile_map.hpp"
#include <iostream>
#include <memory>

#define xstr(x) #x
#define STRINGIFY(x) xstr(x)

static const auto PTM_RATIO = 32.0f;
using namespace ev;

static std::string filepath(const std::string &filename) {
  static const std::string source_directory = STRINGIFY(SOURCE_DIRECTORY);
  return source_directory + "/test/" + filename;
}

class PhysicsSprite
{
 public:
  PhysicsSprite(const ev::Sprite& s) : sprite(s) {}

  void destroyBody(b2World& world) {
    if(body) {
      world.DestroyBody(body);
      body = nullptr;
    }
  }

  void update(float dt) {
    sprite.update(dt);

    if(body) {
      b2Vec2 p = body->GetPosition();
      sprite.position.x = p.x;
      sprite.position.y = p.y;
      sprite.position *= PTM_RATIO;
    }
  }

  void createBody(b2World& world) {
    b2BodyDef bodyDef;
    bodyDef.userData = this;
    body = world.CreateBody(&bodyDef);
  }

  void set_as_circle(float radius) {
    b2PolygonShape shape;
    b2FixtureDef   fixtureDef;

    shape.m_radius = radius / PTM_RATIO;
    fixtureDef.shape = &shape;
  }

 private:
  ev::Sprite sprite;
  b2Body    *body = nullptr;
};

static const glm::vec3 cube_verts[] = {
  {-1.0f, -1.0f, 1.0f},
  { 1.0f, -1.0f, 1.0f},
  { 1.0f,  1.0f, 1.0f},
  {-1.0f,  1.0f, 1.0f},
  // back
  {-1.0f, -1.0f, -1.0f},
  { 1.0f, -1.0f, -1.0f},
  { 1.0f,  1.0f, -1.0f},
  {-1.0f,  1.0f, -1.0f}
};

static const GLushort cube_elements[] = {
  0, 1, 2,
  2, 3, 0,
  1, 5, 6,
  6, 2, 1,
  7, 6, 5,
  5, 4, 7,
  4, 0, 3,
  3, 7, 4,
  4, 5, 1,
  1, 0, 4,
  3, 2, 6,
  6, 7, 3
};

static const std::string cube_frag =
  "varying vec3 f_color;"
   "void main(void) {"
   "gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);"
  "}";

static const std::string cube_vert =
  R"(
attribute vec3 coord3d;
attriubte vec3 v_color;
uniform mat4 mvp;
varying vec3 f_color;

void main(void) {
  gl_Position = mvp * vec4(coord3d, 1.0);
  f_color = v_color;
}
)";

// ev::VertexBuff<glm::vec3> cube_vbo(8);
// ev::VertexBuff<glm::vec3> cube_colors(8);
// ev::VertexBuff<glm::u16vec3, GL_ELEMENT_ARRAY_BUFFER> cube_ibo(8);

class Voxel
{
public:
  glm::vec3 position;
  float     size;
};

class TestBed : public ev::Application {
public:
  TestBed()
      : ev::Application(800, 600, "ev"), world(b2Vec2(0.0f, 9.0f)),
        transform(glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 1.0f, -1.0f)),
        tex(std::make_shared<ev::Texture>(filepath("flappybirds.png"))),
        sheet(filepath("flappybird.json")), batch(tex) {
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, get_width(), get_height());

    bird = sheet.create_sprite({"blue_bird_1", "blue_bird_2", "blue_bird_3"});
    bird.position.x = 400;
    bird.position.y = 300;
    bird.scale = 5.0f;

    background = sheet.create_sprite({"background1"});
    pipes.push_back(sheet.create_sprite({"green_pipe_down"}));
    setupPhysics();

    auto map_texture = std::make_shared<ev::Texture>(filepath("Tile.png"));
    this->map = std::make_unique<TileMap>(128,128,32, map_texture);

    map->set_viewport(Rectangle(0, 0, 800, 600));
  }

  void setupPhysics() {
    auto width = get_width() * 4;
    auto height = get_height() * 4;

    world.SetAllowSleeping(true);
    world.SetContinuousPhysics(true);
    b2BodyDef groundBoundyDef;

    b2Body *worldBox = world.CreateBody(&groundBoundyDef);

    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0, 0), b2Vec2(width, 0));
    worldBox->CreateFixture(&groundBox, 0);

    groundBox.Set(b2Vec2(0, height), b2Vec2(width, height));
    worldBox->CreateFixture(&groundBox, 0);

    groundBox.Set(b2Vec2(0, height), b2Vec2(0, 0));
    worldBox->CreateFixture(&groundBox, 0);

    groundBox.Set(b2Vec2(width, height), b2Vec2(width, 0));
    worldBox->CreateFixture(&groundBox, 0);
  }

  virtual void render() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    map->render(transform);
    ev::SpriteBatch::Filler filler(batch);
    filler.render(background);
    filler.render(bird);

    for (const auto &pipe : pipes) {
      filler.render(pipe);
    }
    batch.render(filler, transform);
  }

  virtual void update(const float dt) override {
    bird.update(dt);
    background.update(dt);
    for (auto &p : pipes) {
      p.update(dt);
    }
    world.Step(dt, 8, 3);
  }

private:
  std::unique_ptr<TileMap> map;
  b2World world;
  glm::mat4 transform;
  std::shared_ptr<ev::Texture> tex;
  ev::SpriteSheet sheet;
  ev::SpriteBatch batch;
  ev::Sprite bird;
  std::vector<ev::Sprite> pipes;
  ev::Sprite background;
};

int main() {
  try {
    TestBed t;

    t.show();
    t.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
