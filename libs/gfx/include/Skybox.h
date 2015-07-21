#pragma once
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <memory>

#include <common/include/Tools.h>
#include <gfx/include/OpenGL_Headers.h>

namespace mkay
{
	class Shader_program;
	class Texture_cube_map;
	class Texture;
  class Camera;

	enum class VBO_skybox_type
		: GLuint
	{
		Position = 0,
		Index = 1,
		Count
	};
  
  class Skybox
  {
  public:
    void init();
		void render(Camera & i_camera);
		
  private:
    Shader_program *m_shader;
		Texture_cube_map *m_texture;

		GLuint m_vbo_id[to_integral(VBO_skybox_type::Count)];
		GLsizei m_triangle_count = 0;

		void buffer_data();
		void draw(Camera & i_camera);
	};
} // namespace mkay

#endif // __SKYBOX_H__