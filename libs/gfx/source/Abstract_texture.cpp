#include <gfx/include/Abstract_texture.h>

#include <common/include/Logger.h>

#include <IL/ilu.h>

namespace mkay 
{
  void save_2d_rgb_image( const char *i_save_file_name, void *i_data
                        , ILuint i_width, ILuint i_height )
  {
    ILuint image_id;
    
    // gen a new il texture object
    ilGenImages(1, &image_id);
    ilBindImage(image_id);
    
    ilTexImage(i_width, i_height, 1, 3, IL_RGB, IL_UNSIGNED_SHORT, i_data);
  
    ilSave(IL_PSD, i_save_file_name);

    ilDeleteImages(1, &image_id);
  }

  Abstract_texture::Abstract_texture()
    : m_anisotropic_filter_level(get_max_anisotropy())
  {
  }
  
  Abstract_texture::~Abstract_texture()
  {
  }
  
  GLubyte * Abstract_texture::il_load_image_data(std::string const & i_full_path, ILuint & o_image_id)
  {
    loginf << "loading image data from " << i_full_path << std::endl;

    // gen a new il texture object
    ilGenImages(1, &o_image_id);
    ilBindImage(o_image_id);
    if( !ilLoadImage(i_full_path.c_str()) ) 
    { 
      ilDeleteImages(1, &o_image_id);
      logerr << "Texture: could not load image from '" << i_full_path << "' - " << iluErrorString(ilGetError()) << std::endl;
      return nullptr;
    }

    return ilGetData();
  }
} // namespace mkay