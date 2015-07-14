#include <gfx/include/Shader_program.h>
#include <gfx/include/Texture_cube_map.h>
#include <common/include/Logger.h>

#include <IL/il.h>

#include <cstdint>

using namespace std;

namespace mkay
{
  Texture_cube_map::Texture_cube_map()
  {
    m_wrap_s = m_wrap_t = m_wrap_r = GL_CLAMP_TO_EDGE;
  }
	  
  void Texture_cube_map::set(Shader_program* i_shader, GLenum i_tex_unit, const char* i_shader_var)
  {
    glActiveTexture(i_tex_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex_id);

    glUniform1i( i_shader->get_uniform_location(i_shader_var), (i_tex_unit-GL_TEXTURE0) );

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrap_r);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		if (m_anisotropic_filter_level > 0.0f)
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropic_filter_level);
  }
  
  void Texture_cube_map::unset()
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }
  
  void Texture_cube_map::load_from(std::string const &i_path)
  {
    const size_t pic_no = 6;
    const char *names[pic_no] =
    {
      "positive_x.png",
      "negative_x.png",
      "positive_y.png",
      "negative_y.png",
      "positive_z.png",
      "negative_z.png"
    };

    // generate & bind a opengl texture
    glGenTextures(1, &m_tex_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex_id);

    for(size_t i=0; i<pic_no; ++i)
    {
      ILuint image_id = 0;
      GLubyte * data = il_load_image_data( i_path + "/" + names[i], image_id );

      GLint image_format = ilGetInteger(IL_IMAGE_FORMAT);

      glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, image_format,
                    ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                    0, image_format, GL_UNSIGNED_BYTE, data );
      
      // delete the il image object  
      ilDeleteImages(1, &image_id);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrap_r);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    loginf << "Generating CubeMap Texture from '" << i_path << "' complete!" << endl;
  }
} // namespace mkay