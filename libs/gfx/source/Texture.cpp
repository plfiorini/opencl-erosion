#include <gfx/include/Texture.h>

#include <gfx/include/Shader_program.h>

namespace mkay
{
	void Texture::set(Shader_program* i_shader, GLenum i_tex_unit, const char* i_shader_var)
	{
		glActiveTexture(i_tex_unit);

		glBindTexture(GL_TEXTURE_2D, m_tex_id);

		glUniform1i(i_shader->get_uniform_location(i_shader_var), (i_tex_unit - GL_TEXTURE0));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_anisotropic_filter_level > 0.0f)
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropic_filter_level);
  }
  
  void Texture::unset()
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
  void Texture::load_from(std::string const &i_full_path)
  {
  }
} // namespace mkay