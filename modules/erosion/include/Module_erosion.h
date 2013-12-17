#pragma once
#ifndef __MODULE_EROSION_H__
#define __MODULE_EROSION_H__

#include <module/include/Module.h>

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
    
    Module_erosion() = default;
    
  private:
    mutable bool m_exit_flag = false;
  };
} // namespace mkay

#endif // __MODULE_EROSION_H__