#pragma once
#ifndef __MODULE_EROSION_H__
#define __MODULE_EROSION_H__

#include <memory>

#include <module/include/Module.h>

#include <gfx/include/SDL_window_manager.h>
#include <gfx/include/SDL_event_manager.h>
#include <gfx/include/Camera.h>
#include <gfx/include/Point_light.h>
#include <gfx/include/Skybox.h>
#include <gfx/include/Box.h>

#include <cl/include/CL_manager.h>

namespace mkay
{
  class Shader_program;
  
  class Module_erosion 
    : public Module
  {
  public:
    virtual void get_options_description(boost::program_options::options_description & o_description);
    virtual void configure(boost::program_options::variables_map const & i_variables);
    virtual void step();
    virtual void shutdown();
    virtual bool requests_exit() { return m_event_manager.received_quit_event(); }
    
    Module_erosion();
    virtual ~Module_erosion();
    
    /// static module type name function
    static const char * get_module_type() { return "erosion"; }
    
  private:
    const int32_t c_default_width = 1280, c_default_height = 768;
    const std::string c_default_search_path = "..";
    
    SDL_window_manager m_window_manager;
    SDL_event_manager m_event_manager;
    CL_manager m_cl_manager;
    
    Shader_program * m_shader;
    
    Camera m_camera;
    
    Point_light m_light;
    
    Skybox m_skybox;
    Box m_geo_object;
    
    void render_scene();
  };
  
  typedef std::shared_ptr<Module> Module_ptr_t;
} // namespace mkay

#endif // __MODULE_EROSION_H__