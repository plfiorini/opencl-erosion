#pragma once
#ifndef __MATERIAL_FACTORY_H__
#define __MATERIAL_FACTORY_H__

#include <gfx/include/Abstract_material.h>

namespace mkay
{
  class Material_factory
  {
  public:
    static Abstract_material * create(Material_type i_type);
    
  private:
    Material_factory() = delete;
    Material_factory(const Material_factory &) = delete;
    Material_factory & operator=(const Material_factory &) = delete;
  };
} // namespace mkay

#endif // __MATERIAL_FACTORY_H__