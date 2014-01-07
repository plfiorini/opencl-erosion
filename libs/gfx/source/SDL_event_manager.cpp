#include <gfx/include/SDL_event_manager.h>
#include <gfx/include/SDL_window_manager.h>
#include <gfx/include/Camera.h>
#include <common/include/Logger.h>
#include <common/include/glm_definitions.h>

#include <SDL2/SDL.h>
#include <boost/iterator/iterator_concepts.hpp>

using namespace std;

namespace mkay
{ 
  SDL_event_manager::SDL_event_manager()
  {
    for ( auto & elem : m_movement )
    {
      elem = false;
    }
  }

  void SDL_event_manager::configure(SDL_window_manager *i_window_manager, Camera *i_camera)
  {
    m_window_manager = i_window_manager;
    m_current_camera = i_camera;
    
    m_mouse_old_position = m_window_manager->get_center_position();
    
    //SDL_SetRelativeMouseMode(SDL_TRUE);
  }
  
  void SDL_event_manager::handle_event_loop()
  {   
    SDL_Event event;
    // Check for messages
    while (SDL_PollEvent(&event))
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
      case SDL_MOUSEBUTTONDOWN:
        loginf << "mouse button down: " << static_cast<uint32_t>(event.button.button) 
               << " - state: " << static_cast<uint32_t>(event.button.state) << endl;
        if ( event.button.button == SDL_BUTTON_RIGHT )
        {
          m_mouse_rightclick = true;
          SDL_ShowCursor(0);
        }
        break;
      case SDL_MOUSEBUTTONUP:
        loginf << "mouse button up: " << static_cast<uint32_t>(event.button.button) 
               << " - state: " << static_cast<uint32_t>(event.button.state) << endl;
        if ( event.button.button == SDL_BUTTON_RIGHT )
        {
          m_mouse_rightclick = false;
          SDL_ShowCursor(1);
        }
        break;
      case SDL_MOUSEMOTION:
        loginf << "mouse motion" << endl;
        if ( m_mouse_rightclick )
        {
          handle_mouse_motion();
        }
      case SDL_WINDOWEVENT:
        handle_window_event(event.window);
        break;
      }
    }
  }
  
  void SDL_event_manager::handle_key_event(SDL_KeyboardEvent const & i_key_event)
  {
    bool movement = false;
    bool down = (i_key_event.type==SDL_KEYDOWN);
    
    logdeb << "key was pressed/released: " << i_key_event.keysym.scancode 
           << " - state: " << down << endl;
    
    // key actions only on key down
    if ( i_key_event.type == SDL_KEYDOWN )
    {
      switch(i_key_event.keysym.scancode)
      {
      case SDL_SCANCODE_ESCAPE:
        loginf << "quit" << endl;
        m_quit_event = true;
        break;
      case SDL_SCANCODE_H:
        m_current_camera->return_home();
        loginf << "returning camera to home position: " << *m_current_camera << endl;
        break;
      default:
        break;
      }
    }

    switch(i_key_event.keysym.scancode)
    {
    case SDL_SCANCODE_A:
      m_movement[static_cast<size_t>(Movement::Left)] = down; movement = true; break;
    case SDL_SCANCODE_D:
      m_movement[static_cast<size_t>(Movement::Right)] = down; movement = true; break;
    case SDL_SCANCODE_W:
      m_movement[static_cast<size_t>(Movement::Forward)] = down; movement = true; break;
    case SDL_SCANCODE_S:
      m_movement[static_cast<size_t>(Movement::Backward)] = down; movement = true; break;
    case SDL_SCANCODE_LSHIFT:
      m_movement[static_cast<size_t>(Movement::Up)] = down; movement = true; break;
    case SDL_SCANCODE_SPACE:
      m_movement[static_cast<size_t>(Movement::Down)] = down; movement = true; break;      
    default: break;
    }
    
    if ( movement )
    {
      glm::vec3 move_vector{
        m_movement_speed*m_movement[static_cast<size_t>(Movement::Left)] 
          - m_movement_speed*m_movement[static_cast<size_t>(Movement::Right)],
        m_movement_speed*m_movement[static_cast<size_t>(Movement::Forward)] 
          - m_movement_speed*m_movement[static_cast<size_t>(Movement::Backward)],
        m_movement_speed*m_movement[static_cast<size_t>(Movement::Up)] 
          - m_movement_speed*m_movement[static_cast<size_t>(Movement::Down)]
      };
      
      logdeb << "move vector: " << move_vector << endl;
      
      m_current_camera->move(move_vector);
    }
  }
  
  void SDL_event_manager::handle_window_event(const SDL_WindowEvent& i_window_event)
  {
    switch(i_window_event.event)
    {
    case SDL_WINDOWEVENT_RESIZED:
      loginf << "window resized to " << i_window_event.data1 << "x" << i_window_event.data2 << endl;
      m_window_manager->on_resize(glm::ivec2{i_window_event.data1, i_window_event.data2});
      m_current_camera->set_aspect_ratio(i_window_event.data1, i_window_event.data2);
      break;
    }
  }

  void SDL_event_manager::handle_mouse_motion()
  {
    glm::ivec2 current_position;
    SDL_GetMouseState(&current_position.x, &current_position.y);
    
    glm::vec2 delta = glm::vec2(current_position) - glm::vec2(m_mouse_old_position);    
    m_current_camera->rotate(delta.yx);
              
    glm::ivec2 center = m_window_manager->get_center_position();
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE); 
    SDL_WarpMouseInWindow(m_window_manager->get_window(), center.x, center.y);
    SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
    
    m_mouse_old_position = center;
  }
} // namespace mkay