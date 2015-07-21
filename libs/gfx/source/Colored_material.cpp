#include <gfx/include/Colored_material.h>
#include <gfx/include/Shader_program.h>
#include <gfx/include/Ressource_manager.h>

#include <color_shader/include/vars.hgl>

namespace mkay 
{
  Colored_material::Colored_material(Shader_program *i_shader_hint /* = nullptr*/)
  {
    if ( !i_shader_hint )
    {
      m_shader = RESMAN.get<Shader_program>("color_shader");
    }
		else
		{
			m_shader = i_shader_hint;
		}
  }

  void Colored_material::use()
  {
    glUniform4fv(m_shader->get_uniform_location(v4_material_color), 1, glm::value_ptr(m_material_color) );
    glUniform1f(m_shader->get_uniform_location(f_phong_coeff), m_phong_coeff);
    glUniform1f(m_shader->get_uniform_location(f_ignore_lighting), m_lighting);
  }
} // namespace mkay