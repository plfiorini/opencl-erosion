#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/gl.h>

#include <gfx/include/Abstract_texture.h>

namespace mkay
{
  class Texture
    : public Abstract_texture
  {    
  public:
    Texture() = default;
    virtual ~Texture() = default;
    
  protected: 
    friend class Ressource_manager;
    
    virtual void load_from(std::string const &i_full_path);
    
    GLint m_wrap_r;
  };
} // namespace mkay

#endif // __TEXTURE_H__