#pragma once
#include <string>
#include "ev.hpp"

namespace ev {

class Shader {
 public:
  Shader(const std::string& source, GLenum type);
  Shader(Shader&& other) : id(other.id) {
    other.id = 0;
  }
  Shader& operator=(Shader&& other) {
    id = other.id;
    other.id = 0;
    return *this;
  }

  virtual ~Shader();

  GLuint get_id() { return id; }

 private:
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  GLuint id = 0;
};

class ShaderProgram
{
 public:
  ShaderProgram(const std::string& vertex_source,
                const std::string& frag_source);
  ShaderProgram(ShaderProgram&& other) : id(other.id), vertex(std::move(other.vertex)),
                                         fragment(std::move(other.fragment)) {
    other.id = 0;
  }
  ShaderProgram& operator=(ShaderProgram&& other) {
    vertex = std::move(other.vertex);
    fragment = std::move(other.fragment);
    id = other.id;
    other.id = 0;
    return *this;
  }

  virtual ~ShaderProgram();

  void use() const;

  GLint get_attrib_loc(const std::string& s) const {
    GLint i =  glGetAttribLocation(id, s.c_str());
    #ifndef NDEBUG
    if( i < 0 ) {
      throw std::runtime_error("inavlid attribute location");
    }
    #endif
    return i;
  }
  GLint get_uniform_loc(const std::string& s) const {
    GLint i = glGetUniformLocation(id, s.c_str());
    #ifndef NDEBUG
    if( i < 0 ) {
      throw std::runtime_error("invalid uniform location");
    }
    #endif
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
