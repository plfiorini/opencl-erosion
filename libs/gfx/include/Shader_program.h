#pragma once
#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#define SHADER_VAR(io, type, name) \
  const char *name = "name"

namespace mkay
{ 
  class Shader_program
  {
  public:
    Shader_program(const std::string & i_name);
    
    void add_source(const char *i_path, GLenum i_type);    
    void build();
    
    void use() { glUseProgram(m_program_id); }
    GLint get_uniform_location(const char *i_name);
    
  private:
    GLuint m_program_id;
    std::string m_name;
    std::map<GLenum, std::string> m_shader_source;
    std::vector<GLuint> m_shader_ids;
    
    void compile_shader(GLenum i_type, std::string & i_source);
    void link_program();
  };
} // namespace mkay

#endif // __SHADER_PROGRAM_H__