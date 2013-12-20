#include <gfx/include/Texture_cube_map.h>
#include <common/include/Logger.h>

#include <IL/il.h>

#include <cstdint>

using namespace std;

namespace mkay
{
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

    glEnable(GL_TEXTURE_CUBE_MAP);
    // generate & bind a opengl texture
    glGenTextures(1, &m_tex_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex_id);

    for(size_t i=0; i<pic_no; ++i)
    {
      //GLubyte * data = LoadImageData((i_path + "/" + names[i]).c_str());
      GLubyte *data;

      glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, ilGetInteger(IL_IMAGE_BPP), 
                    ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                    0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrap_r);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glDisable(GL_TEXTURE_CUBE_MAP);

    // delete the il image object  
    //ilDeleteImages(1, &m_ImageID); m_ImageID = 0;

    loginf << "Generating CubeMap Texture from '" << i_path << "' complete!" << endl;
  }
} // namespace mkay