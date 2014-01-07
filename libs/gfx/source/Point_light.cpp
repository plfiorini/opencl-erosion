#include <gfx/include/Point_light.h>

#include <gfx/include/Shader_program.h>
#include <common_shader/include/vars.hgl>

namespace mkay 
{
  void Point_light::let_there_be_light(Shader_program* i_active_shader)
  {
    glUniform3fv(i_active_shader->get_uniform_location(light_position), 1, glm::value_ptr(m_position) );
    glUniform4fv(i_active_shader->get_uniform_location(light_ambient_color), 1, glm::value_ptr(m_ambient) );
    glUniform4fv(i_active_shader->get_uniform_location(light_diffuse_color), 1, glm::value_ptr(m_diffuse) );
    glUniform4fv(i_active_shader->get_uniform_location(light_specular_color), 1, glm::value_ptr(m_specular) );
    glUniform1f(i_active_shader->get_uniform_location(light_range), m_range);
    glUniform1f(i_active_shader->get_uniform_location(light_constant_attenuation), m_const_attenuation);
    glUniform1f(i_active_shader->get_uniform_location(light_linear_attenuation), m_linear_attenuation);
    glUniform1f(i_active_shader->get_uniform_location(light_quadratic_attenuation), m_quadratic_attenuation);
  }
} // namespace mkay