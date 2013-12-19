#pragma once
#ifndef __SDL_INPUT_MANAGER_H__
#define __SDL_INPUT_MANAGER_H__

struct SDL_KeyboardEvent;

namespace mkay 
{
  enum class Movement
  {
    Left = 0,
    Right,
    Forward,
    Backward,
    Up,
    Down
  };
  
  class SDL_input_manager
  {
  private:
    static const int movement_count = sizeof(Movement);
  public:
    SDL_input_manager();
    
    void configure();
    
    void handle_event_loop();
    
    bool received_quit_event() { return m_quit_event; }
    
  private:    
    bool m_quit_event;
    bool m_movement[movement_count];
    
    void handle_key_event(SDL_KeyboardEvent const & i_key_event);
  };
} // namespace mkay

#endif // __SDL_INPUT_MANAGER_H__