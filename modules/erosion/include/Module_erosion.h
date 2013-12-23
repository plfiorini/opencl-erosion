#pragma once
#ifndef __MODULE_EROSION_H__
#define __MODULE_EROSION_H__

#include <memory>

#include <module/include/Module.h>
#include <gfx/include/SDL_window_manager.h>
#include <gfx/include/Camera.h>
#include <gfx/include/Skybox.h>
#include <input/include/SDL_input_manager.h>
#include <cl/include/CL_manager.h>

namespace mkay
{
  class Module_erosion 
    : public Module
  {
  public:
    virtual void get_options_description(boost::program_options::options_description & o_description);
    virtual void configure(boost::program_options::variables_map const & i_variables);
    virtual void step();
    virtual void shutdown();
    virtual bool requests_exit() { return m_input_manager.received_quit_event(); }
    
    Module_erosion();
    virtual ~Module_erosion();
    
  private:
    SDL_window_manager m_window_manager;
    SDL_input_manager m_input_manager;
    CL_manager m_cl_manager;
    
    Camera m_camera;
    
    Skybox m_skybox;
  };
  
  typedef std::shared_ptr<Module> Module_ptr_t;
} // namespace mkay

#endif // __MODULE_EROSION_H__