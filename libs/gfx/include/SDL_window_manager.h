#pragma once
#ifndef __SDL_HELPER_H__
#define __SDL_HELPER_H__

#include <string>
#include <memory>

#include <gl/glew.h>
#include <SDL2/SDL.h>

#include <common/include/glm_definitions.h>

#ifdef _DEBUG
  #define GL_CHECK( _msg ) \
    { \
      GLenum error = glGetError();  \
      if( error != GL_NO_ERROR ) \
      { \
        logerr << _msg << ": GL Error: " << error << " - " << gluErrorString( error ) << std::endl; \
      } \
    }
#else
  #define GL_CHECK( _msg )
#endif

namespace mkay
{
  class SDL_window_manager
  {
  public:
    SDL_window_manager();
    ~SDL_window_manager();

    void create( 
      std::string const & i_window_name,
      glm::ivec2 const & i_window_size
    );
    void destroy();
        
    void swap_buffers() 
    {
      SDL_GL_SwapWindow(m_window);
    }
    
    const SDL_GLContext & get_context() const { return m_context; }
    
    SDL_Window * get_window() const { return m_window; }
    
    glm::ivec2 get_center_position() { return m_window_size/2; }

    void on_resize(glm::ivec2 const & i_new_window_size);
    
  private:
    bool m_initialized;
    SDL_Window *m_window;
    SDL_GLContext m_context;
    std::string m_window_name;
    glm::ivec2 m_window_size;
  };

} // namespace mkay

#endif // __SDL_HELPER_H__