#include <input/include/SDL_input_manager.h>
#include <common/include/Logger.h>

#include <SDL2/SDL.h>
#include <boost/iterator/iterator_concepts.hpp>

using namespace std;

namespace mkay
{
  SDL_input_manager::SDL_input_manager()
    : m_quit_event(false)
    , m_movement{false}
  {
  }
  
  void SDL_input_manager::configure()
  {
    SDL_SetRelativeMouseMode(SDL_TRUE);
  }
  
  void SDL_input_manager::handle_event_loop()
  {   
    SDL_Event event;
    // Check for messages
    if (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT: // Check for the quit message
        // Quit the program
        loginf << "quit" << endl;
        m_quit_event = true;
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        handle_key_event(event.key);
        break;
      }
    }
  }
  
  void SDL_input_manager::handle_key_event(SDL_KeyboardEvent const & i_key_event)
  {
    bool down = (i_key_event.type==SDL_KEYDOWN);
    
    logdeb << "key was pressed: " << i_key_event.keysym.scancode 
           << " - state: " << down << endl;
    
    switch(i_key_event.keysym.scancode)
    {
    case SDL_SCANCODE_ESCAPE:
      loginf << "quit" << endl;
      m_quit_event = true;
      break;
    case SDL_SCANCODE_A:
      m_movement[static_cast<size_t>(Movement::Left)] = down; break;
    case SDL_SCANCODE_D:
      m_movement[static_cast<size_t>(Movement::Right)] = down; break;
    case SDL_SCANCODE_W:
      m_movement[static_cast<size_t>(Movement::Forward)] = down; break;
    case SDL_SCANCODE_S:
      m_movement[static_cast<size_t>(Movement::Backward)] = down; break;
    case SDL_SCANCODE_LSHIFT:
      m_movement[static_cast<size_t>(Movement::Up)] = down; break;
    case SDL_SCANCODE_SPACE:
      m_movement[static_cast<size_t>(Movement::Down)] = down; break;      
    default: break;
    }
  }
} // namespace mkay