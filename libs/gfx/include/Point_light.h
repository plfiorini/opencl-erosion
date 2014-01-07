#pragma once
#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <common/include/glm_definitions.h>

namespace mkay
{
  class Shader_program;
  
  class Point_light
  {
  public:
    Point_light() = default;

    void set_position(const glm::vec3 &i_Position) { m_position = i_Position; }
    const glm::vec3 & get_position(void) { return m_position; }

    void set_ambient_color(const glm::vec4 &i_Ambient) { m_ambient = i_Ambient; }
    const glm::vec4 & get_ambient_color(void) { return m_ambient; }

    void set_diffuse_color(const glm::vec4 &i_Diffuse) { m_diffuse = i_Diffuse; }
    const glm::vec4 & get_diffuse_color(void) { return m_diffuse; }

    void set_specular_color(const glm::vec4 &i_Specular) { m_specular = i_Specular; }
    const glm::vec4 & get_specular_color(void) { return m_specular; }

    void set_range(const float i_Range) { m_range = i_Range; }
    const float get_range(void) { return m_range; }
    
    void set_const_attenuation(const float catten) { m_const_attenuation = catten; }
    const float get_const_attenuation(void) { return m_const_attenuation; }

    void set_linear_attenuation(const float latten) { m_linear_attenuation = latten; }
    const float get_linear_attenuation(void) { return m_linear_attenuation; }

    void set_quadratic_attenuation(const float qatten) { m_quadratic_attenuation = qatten; }
    const float get_quadratic_attenuation(void) { return m_quadratic_attenuation; }

    void let_there_be_light(Shader_program *i_active_shader);
    
  private:
    glm::vec3 m_position = glm::vec3{0.0f, 0.0f, 0.0f};
    
    glm::vec4 m_ambient = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 m_diffuse = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 m_specular = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
    
    float m_range = 100.0f;
    float m_const_attenuation = 0.0f;
    float m_linear_attenuation = 1.0f;
    float m_quadratic_attenuation = 0.0f;
  };
} // namespace mkay

#endif // __POINT_LIGHT_H__