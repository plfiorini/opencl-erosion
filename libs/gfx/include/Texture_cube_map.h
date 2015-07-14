#pragma once
#ifndef __TEXTURE_CUBE_MAP_H__
#define __TEXTURE_CUBE_MAP_H__

#include <gfx/include/OpenGL_Headers.h>

#include <gfx/include/Abstract_texture.h>

namespace mkay
{
  class Texture_cube_map
    : public Abstract_texture
  {   
  public:
    Texture_cube_map();
    virtual ~Texture_cube_map() = default;
    
    virtual void set(Shader_program* i_shader, GLenum i_tex_unit, const char* i_shader_var);
    virtual void unset();
    
  protected:
    friend class Ressource_manager;
    
    virtual void load_from(std::string const &i_full_path);
    
    GLint m_wrap_r;
  };
} // namespace mkay

#endif // __TEXTURE_CUBE_MAP_H__