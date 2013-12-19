#include <gfx/include/Texture_cube_map.h>

namespace mkay
{
  void Texture_cube_map::load_from(std::string const &i_path)
  {
    const char *names[6] =
    {
      "positive_x.png",
      "negative_x.png",
      "positive_y.png",
      "negative_y.png",
      "positive_z.png",
      "negative_z.png"
    };

    glEnable(GL_TEXTURE_CUBE_MAP);
    // generate & bind a opengl texture
    glGenTextures(1, &m_TexID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID);

    for(boost::int32_t i=0; i<6; ++i)
    {
      GLubyte * data = LoadImageData((m_Path + names[i]).c_str());

      glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, ilGetInteger(IL_IMAGE_BPP), 
                    ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                    0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_WrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_WrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_WrapR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glDisable(GL_TEXTURE_CUBE_MAP);

    // delete the il image object  
    ilDeleteImages(1, &m_ImageID); m_ImageID = 0;

    loginf << "Generating CubeMap Texture from '" << m_Path << "' complete!" << endl;
  }
} // namespace mkay