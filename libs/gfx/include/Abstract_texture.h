#pragma once
#ifndef __ABSTRACT_TEXTURE_H__
#define __ABSTRACT_TEXTURE_H__

#include <string>

#include <GL/gl.h>
#include <IL/il.h>

class Shader_program;

namespace mkay
{
  void save_2d_rgb_image( const char *i_save_file_name, void *i_data
                        , ILuint i_width, ILuint i_height );
  
  class Abstract_texture
  {
  public:
    virtual ~Abstract_texture() = 0;
    
    void set(Shader_program *i_shader, GLenum i_tex_unit, const char *i_shader_var);
    void unset();
    
    /// GL_ADD, GL_REPLACE, GL_BEND, GL_MODULATE...
    void set_env_mode(GLuint i_env_mode) { m_env_mode = i_env_mode; }
    
    void set_anisotropic_filtering(GLfloat i_level) { m_anisotropic_filter_level = i_level; }

    void set_wrap_s(GLuint i_wrap_s) { m_wrap_s = i_wrap_s; }
    void set_wrap_t(GLuint i_wrap_t) { m_wrap_t = i_wrap_t; }

    GLint get_color_channels() { return m_color_channels; }
    GLint get_bit_per_channel() { return m_bit_per_channel; }
    
  protected:
    friend class Ressource_manager;
    
    virtual void load_from(std::string const &i_full_path) = 0;

    GLuint m_tex_id = 0;
    
    GLuint m_env_mode;
    GLint m_wrap_s, m_wrap_t;
    GLfloat m_anisotropic_filter_level;
    
    GLint m_color_channels;
    GLint m_bit_per_channel;    
  };
} // namespace mkay

#endif // __ABSTRACT_TEXTURE_H__