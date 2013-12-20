#pragma once
#ifndef __TEXTURE_CUBE_MAP_H__
#define __TEXTURE_CUBE_MAP_H__

#include <GL/gl.h>

#include <gfx/include/Abstract_texture.h>

namespace mkay
{
  class Texture_cube_map
    : public Abstract_texture
  {   
  public:
    Texture_cube_map() = default;
    virtual ~Texture_cube_map() = default;
    
  protected:
    friend class Ressource_manager;
    
    virtual void load_from(std::string const &i_full_path);
    
    GLint m_wrap_r;
  };
} // namespace mkay

#endif // __TEXTURE_CUBE_MAP_H__