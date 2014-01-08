#pragma once
#ifndef __CUBE_H__
#define __CUBE_H__

#include <gfx/include/Abstract_geometric_object.h>

namespace mkay
{
  class Abstract_material;
  
  class Cube
    : public Abstract_geometric_object
  {
  public:
    Cube(float i_size)
      : m_size(i_size)
    {
    }
    ~Cube() = default;

  protected:
    virtual void internal_buffer_data(Abstract_material_ptr i_material);
    
  private:
    float m_size = 1.0f;
  };
} // namespace mkay

#endif // __CUBE_H__