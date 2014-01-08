#pragma once
#ifndef __COLORED_MATERIAL_H__
#define __COLORED_MATERIAL_H__

#include <memory>

#include <common/include/glm_definitions.h>
#include <gfx/include/Abstract_material.h>

namespace mkay
{
  class Shader_program;
  
  class Colored_material
    : public Abstract_material
  {
  public:
    Colored_material(Shader_program *i_shader_hint = nullptr);
    
    void set_material_color(glm::vec4 const & i_material_color) { m_material_color = i_material_color; }
    void set_phong_coefficient(float i_phong_coeff) { m_phong_coeff = i_phong_coeff; }
    void ignore_lighting(bool i_ignore) { m_lighting = (i_ignore)?0.0f:1.0f; }

    virtual void use();
    
  protected:
    glm::vec4 m_material_color = glm::vec4{1.0f};
    float m_phong_coeff = 4.0f;
    float m_lighting = 1.0f;

    friend class Material_factory;
  };
  
  typedef std::shared_ptr<Colored_material> Colored_material_ptr;
} // namespace mkay

#endif // __COLORED_MATERIAL_H__