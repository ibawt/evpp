#include "ev.hpp"
#include "application.hpp"

class TestBed : public ev::Application
{
 public:
  TestBed() : ev::Application(800,600, "ev") {}
  virtual void render() {
  }

  virtual void update(const float dt) {
  }
};

int main(int argc, char **argv)
{

  TestBed t;

  t.show();

  return 0;
}
