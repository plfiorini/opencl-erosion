#pragma once
#ifndef __BOX_H__
#define __BOX_H__

#include <gfx/include/Abstract_geometric_object.h>

namespace mkay
{
  class Shader_program;
  
  class Box
    : public Abstract_geometric_object
  {
  public:
    Box(float i_size)
      : m_size(i_size)
    {
    }
    ~Box() = default;
    
  protected:
    virtual void internal_initialize_data_structures(Shader_program *i_shader);
    
  private:
    float m_size;
  };
} // namespace mkay

#endif // __BOX_H__