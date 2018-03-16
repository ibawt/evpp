#include "shader_program.hpp"
#include <vector>

namespace ev {

Shader::Shader(const std::string& source, GLenum type)
{
  id = glCreateShader(type);
  auto s = source.c_str();
  glShaderSource(id, 1, &s, 0);
  glCompileShader(id);

  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);

  if( status == GL_FALSE) {
    GLint loglength;

    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglength);

    std::vector<char> log(loglength);

    glGetShaderInfoLog(id, loglength, &loglength, log.data());
    log.resize(loglength);

    throw EVException(std::string(log.begin(), log.end()));
  }
}

Shader::~Shader() {
  if(id) {
    glDeleteShader(id);
    id = 0;
  }
}

ShaderProgram::ShaderProgram(const std::string& vertex_source,
                             const std::string& frag_source) :
    id(0),
    vertex(vertex_source, GL_VERTEX_SHADER),
    fragment(frag_source, GL_FRAGMENT_SHADER)

{
  id = glCreateProgram();
  glAttachShader(id, vertex.get_id());
  glAttachShader(id, fragment.get_id());
  glLinkProgram(id);

  GLint status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if( status == GL_FALSE) {
    GLint loglength;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &loglength);

    std::vector<char> log(loglength);

    glGetProgramInfoLog(id, loglength, &loglength, log.data());
    log.resize(loglength);

    throw EVException(std::string(log.begin(), log.end()));
  }
}

ShaderProgram::~ShaderProgram()
{
  if(id) {
    glDeleteProgram(id);
    id = 0;
  }
}

void ShaderProgram::use() const
{
  glUseProgram(id);
}

}
