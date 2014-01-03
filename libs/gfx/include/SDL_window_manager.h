#pragma once
#ifndef __SDL_HELPER_H__
#define __SDL_HELPER_H__

#include <string>
#include <SDL2/SDL.h>

#include <gfx/include/Dimension.h>

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
      dimension32_t const & i_window_size
    );
    void destroy();
    
    void swap_buffers() 
    {
      SDL_GL_SwapWindow(m_window);
    }
    
    const SDL_GLContext & get_context() const { return m_context; }

  private:
    bool m_initialized;
    SDL_Window *m_window;
    SDL_GLContext m_context;
    std::string m_window_name;
    dimension32_t m_window_size;
  };

} // namespace mkay

#endif // __SDL_HELPER_H__