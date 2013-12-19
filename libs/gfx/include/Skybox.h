#pragma once
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <memory>

#include <gfx/include/Shader_program.h>
#include <gfx/include/Texture_cube_map.h>

namespace mkay
{
  class Camera;
  
  class Skybox
  {
  public:   
    void init();
    void render(Camera & i_camera);
    
  private:
    std::unique_ptr<Shader_program> m_shader;
    std::unique_ptr<Texture_cube_map> m_texture;
    
  };
} // namespace mkay

#endif // __SKYBOX_H__