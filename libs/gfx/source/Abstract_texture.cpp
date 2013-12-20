#include <gfx/include/Abstract_texture.h>

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
    
  Abstract_texture::~Abstract_texture()
  {
  }
} // namespace mkay