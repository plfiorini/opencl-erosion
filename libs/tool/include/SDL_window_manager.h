#pragma once
#ifndef __SDL_HELPER_H__
#define __SDL_HELPER_H__

#include <string>
#include <SDL2/SDL.h>

#include <tool/include/Dimension.h>

namespace mkay
{
  class SDL_window_manager
  {
  public:
    SDL_window_manager(
      std::string const & i_window_name,
      dimension32_t const & i_window_size
    );
    ~SDL_window_manager();

    void create();
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