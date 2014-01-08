#pragma once
#ifndef __ABSTRACT_MATERIAL_H__
#define __ABSTRACT_MATERIAL_H__

#include <memory>

#include <common/include/Mkay_exception.h>

namespace mkay
{
  class Shader_program;
  
  enum class Material_type
  {
    Colored,
    Textured
  };
  
  class Material_exception
    : public Mkay_exception
  {
  };
  
  class Abstract_material
  {
  public:
    virtual ~Abstract_material() = 0;
    
    Shader_program * get_shader() const { return m_shader; }
    
    virtual void use() = 0;
    
  protected:
    Shader_program * m_shader = nullptr;
    
    Abstract_material(Shader_program *i_shader_hint = nullptr) 
      : m_shader(i_shader_hint)
    {
    }
    
    friend class Material_factory;
  };
  
  typedef std::shared_ptr<Abstract_material> Abstract_material_ptr;
} // namespace mkay

#endif // __ABSTRACT_MATERIAL_H__