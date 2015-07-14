#pragma once
#ifndef __SDL_EVENT_MANAGER_H__
#define __SDL_EVENT_MANAGER_H__

#include <cstddef>

#include <common/include/Tools.h>
#include <common/include/glm_definitions.h>

struct SDL_KeyboardEvent;
struct SDL_WindowEvent;
struct SDL_MouseMotionEvent;

namespace mkay 
{
  class Camera;
  class SDL_window_manager;
	  
  enum class Movement
  {
    Left      = 0,
    Right     = 1,
    Forward   = 2,
    Backward  = 3,
    Up        = 4,
    Down      = 5,
    Count     = 6
  };
  
  class SDL_event_manager
  {
  public:
    SDL_event_manager();
    
    /// configure takes the current window manager as parameter
    /// because it needs it for e.g. resizing the window etc. ...
    void configure(SDL_window_manager *i_window_manager, Camera *i_camera);
    
    void handle_event_loop();
    
    bool received_quit_event() { return m_quit_event; }
    
  private:    
    bool m_quit_event = false;
    
    bool m_movement[to_integral(Movement::Count)];
    float m_movement_speed = 10.0f;
    
    bool m_mouse_rightclick = false;
    glm::ivec2 m_mouse_old_position = glm::ivec2{0, 0};
    
    SDL_window_manager *m_window_manager = nullptr;
    Camera *m_current_camera = nullptr;
    
    void handle_key_event(SDL_KeyboardEvent const & i_key_event);
    void handle_window_event(SDL_WindowEvent const & i_window_event);
    void handle_mouse_motion();
    
    glm::ivec2 center_mouse();
  };
} // namespace mkay

#endif // __SDL_EVENT_MANAGER_H__