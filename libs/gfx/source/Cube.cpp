#include <gfx/include/Cube.h>
#include <gfx/include/Shader_program.h>
#include <gfx/include/Abstract_material.h>

#include <common_shader/include/vars.hgl>
#include <color_shader/include/vars.hgl>

namespace mkay
{
  void Cube::internal_buffer_data(Abstract_material_ptr i_material)
  {
    const GLsizei vertices = 24;
    m_triangle_count = 36;

    // the 4 vertices of a box 
    const GLfloat box[vertices][4] = 
    {
      /* front */
      {  -m_size, -m_size,  m_size, 1.0f  },   
      {  m_size,  -m_size,  m_size, 1.0f  },   
      {  m_size,  m_size, m_size, 1.0f  },   
      {  -m_size, m_size, m_size, 1.0f  },

      /* left */
      {  -m_size, -m_size,  -m_size, 1.0f },   
      {  -m_size, -m_size,  m_size, 1.0f  },   
      {  -m_size, m_size, m_size, 1.0f  },   
      {  -m_size, m_size, -m_size, 1.0f },

      /* right */
      {  m_size,  -m_size,  m_size, 1.0f  },   
      {  m_size,  -m_size,  -m_size, 1.0f },   
      {  m_size,  m_size, -m_size, 1.0f },   
      {  m_size,  m_size, m_size, 1.0f  },

      /* top */
      {  -m_size, m_size, m_size, 1.0f  },   
      {  m_size,  m_size, m_size, 1.0f  },   
      {  m_size,  m_size, -m_size, 1.0f },   
      {  -m_size, m_size, -m_size, 1.0f },

      /* bottom */
      {  -m_size, -m_size,  -m_size, 1.0f },   
      {  m_size,  -m_size,  -m_size, 1.0f },   
      {  m_size,  -m_size,  m_size, 1.0f  },   
      {  -m_size, -m_size,  m_size, 1.0f  },

      /* back */
      {  m_size,  -m_size,  -m_size, 1.0f },   
      {  -m_size, -m_size,  -m_size, 1.0f },   
      {  -m_size, m_size, -m_size, 1.0f },   
      {  m_size,  m_size, -m_size, 1.0f },
    };

    const GLuint boxindices[36] = 
    { 
      /* front */
      0, 1, 2, 
      2, 3, 0,

      /* left */
      4, 5, 6, 
      6, 7, 4,

      /* right */
      8, 9, 10, 
      10, 11, 8,

      /* top */
      12, 13, 14, 
      14, 15, 12,

      /* bottom */
      16, 17, 18, 
      18, 19, 16,

      /* back */
      20, 21, 22, 
      22, 23, 20,
    };

    GLfloat normals[vertices][4];

    // generate the normals
    calculate_normals(&box[0][0], vertices, &boxindices[0], m_triangle_count, &normals[0][0]);

    // buffer all data
    
    Shader_program * material_shader = i_material->get_shader();
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_type::Position)]);
    glBufferData(GL_ARRAY_BUFFER, vertices * 4 * sizeof(GLfloat), box, GL_STATIC_DRAW);
    GLint vertex_attrib_location = material_shader->get_attribute_location(in_position);
    glEnableVertexAttribArray(vertex_attrib_location);
    glVertexAttribPointer(vertex_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_type::Normal)]);
    glBufferData(GL_ARRAY_BUFFER, vertices * 4 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
    GLint normals_attrib_location = material_shader->get_attribute_location(in_normal);
    glEnableVertexAttribArray(normals_attrib_location);
    glVertexAttribPointer(normals_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_type::Index)]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangle_count * sizeof(GL_UNSIGNED_INT), boxindices, GL_STATIC_DRAW);

    // disable pointers & unbind buffers
    glDisableVertexAttribArray(vertex_attrib_location);
    glDisableVertexAttribArray(normals_attrib_location);
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  }
  
//   void Cube::internal_set_shader_vars(Shader_program* i_shader)
//   {
//     glUniform4fv(i_shader->get_uniform_location(v4_material_color), 1, glm::value_ptr(m_material_color) );
//     glUniform1f(i_shader->get_uniform_location(f_phong_coeff), m_phong_coeff);
//     glUniform1f(i_shader->get_uniform_location(f_ignore_lighting), m_lighting);
//   }

} // namespace mkay