#pragma once
#ifndef __SDL_INPUT_MANAGER_H__
#define __SDL_INPUT_MANAGER_H__

struct SDL_KeyboardEvent;

#include <cstddef>

namespace mkay 
{
  class Camera;
  
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
  
  class SDL_input_manager
  {
  public:
    SDL_input_manager();
    
    void configure();
    
    void handle_event_loop();
    
    bool received_quit_event() { return m_quit_event; }
    
    void update_camera(Camera & i_current_camera);
    
  private:    
    bool m_quit_event = false;
    bool m_movement[static_cast<size_t>(Movement::Count)];
    
    float m_movement_speed = 500.0f;
    
    void handle_key_event(SDL_KeyboardEvent const & i_key_event);
  };
} // namespace mkay

#endif // __SDL_INPUT_MANAGER_H__