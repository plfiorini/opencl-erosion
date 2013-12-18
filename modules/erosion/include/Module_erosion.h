#pragma once
#ifndef __MODULE_EROSION_H__
#define __MODULE_EROSION_H__

#include <memory>

#include <module/include/Module.h>
#include <tool/include/SDL_window_manager.h>
#include <tool/include/CL_manager.h>

namespace mkay
{
  class Module_erosion 
    : public Module
  {
  public:
    virtual void configure();
    virtual void step();
    virtual void shutdown();
    virtual bool requests_exit() { return m_exit_flag; }
    
    Module_erosion();
    virtual ~Module_erosion();
    
  private:
    volatile bool m_exit_flag = false;
    std::unique_ptr<SDL_window_manager> m_window_manager;
    std::unique_ptr<CL_manager> m_cl_manager;
  };
} // namespace mkay

#endif // __MODULE_EROSION_H__