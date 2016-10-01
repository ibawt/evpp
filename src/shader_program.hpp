#pragma once
#include <string>
#include "ev.hpp"

namespace ev {

class Shader : Object {
 public:
  Shader(const std::string& source, GLenum type);
  virtual ~Shader();

  GLuint get_id() { return id; }

 private:
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  GLuint id = 0;
};

class ShaderProgram : Object
{
 public:
  ShaderProgram(const std::string& vertex_source,
                const std::string& frag_source);
  virtual ~ShaderProgram();

  void use() const;

  GLint get_attrib_loc(const std::string& s) const {
    GLint i =  glGetAttribLocation(id, s.c_str());
    if( i < 0 ) {
      throw std::runtime_error("inavlid attribute location");
    }
    return i;
  }
  GLint get_uniform_loc(const std::string& s) const {
    GLint i = glGetUniformLocation(id, s.c_str());
    if( i < 0 ) {
      printf("i = %d\n", i);
      throw std::runtime_error("invalid uniform location");
    }
    return i;
  }

 private:
  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;

  GLuint id = 0;
  Shader vertex;
  Shader fragment;
};

}
