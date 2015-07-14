#pragma once
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <memory>

namespace mkay
{
	class Shader_program;
	class Texture_cube_map;
	class Texture;
  class Camera;
  
  class Skybox
  {
  public:   
    void init();
    void render(Camera & i_camera);
    
  private:
    Shader_program *m_shader;
    Texture_cube_map *m_texture;
    
  };
} // namespace mkay

#endif // __SKYBOX_H__