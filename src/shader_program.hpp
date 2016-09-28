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

  GLuint id;
};

class ShaderProgram : Object
{
 public:
  ShaderProgram(const std::string& vertex_source,
                const std::string& frag_source);
  virtual ~ShaderProgram();

  void use() const;

  GLint get_attrib_loc(const std::string& s) const {
    return glGetAttribLocation(id, s.c_str());
  }
  GLint get_uniform_loc(const std::string& s) const {
    return glGetUniformLocation(id, s.c_str());
  }

 private:
  ShaderProgram(const ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;

  GLuint id;
  Shader vertex;
  Shader fragment;
};

}
