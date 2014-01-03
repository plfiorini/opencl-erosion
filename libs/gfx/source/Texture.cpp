#include <gfx/include/Texture.h>

namespace mkay
{
  void Texture::set(Shader_program* i_shader, GLenum i_tex_unit, const char* i_shader_var)
  {

  }
  
  void Texture::unset()
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
  void Texture::load_from(std::string const &i_full_path)
  {
  }
} // namespace mkay