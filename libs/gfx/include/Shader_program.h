#pragma once
#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <common/include/Macros.h>

/// definition macro for shader variables
#define SHADER_VAR(io, type, name) \
  extern const char *name;
  //= STRINGIFY(name)

namespace mkay
{ 
  class Shader_program
  {
  public:
    Shader_program() = default;
    
    void set_source(GLenum i_type, std::string const && i_source);    
    void build(const std::string & i_name);
    
    void use() { glUseProgram(m_program_id); } 
    void disable() { glUseProgram(0); }
      
    GLint get_uniform_location(const char *i_name);
    GLint get_attribute_location(const char *i_name);
    
  private:
    GLuint m_program_id = 0;
    std::string m_name = "not_set";
    std::map<GLenum, std::string> m_shader_source;
    std::vector<GLuint> m_shader_ids;
    
    void compile_shader(GLenum i_type, std::string & i_source);
    void link_program();
  };
} // namespace mkay

#endif // __SHADER_PROGRAM_H__