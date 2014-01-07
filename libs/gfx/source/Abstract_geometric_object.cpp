#include <gfx/include/Abstract_geometric_object.h>
#include <gfx/include/Shader_program.h>

#include <common_shader/include/vars.hgl>

namespace mkay
{
  void calculate_normals(
    const float * i_vertices, const uint32_t i_vertex_count,
    const uint32_t *i_indices, const uint32_t i_index_count,
    float *o_normals ) 
  {
    for(unsigned int i=0; i<i_index_count; i += 3) 
    {
      int i1 = i_indices[i]*4;
      int i2 = i_indices[i+1]*4;
      int i3 = i_indices[i+2]*4;

      glm::vec3 v1( i_vertices[i1 + 0], i_vertices[i1 + 1], i_vertices[i1 + 2] );
      glm::vec3 v2( i_vertices[i2 + 0], i_vertices[i2 + 1], i_vertices[i2 + 2] );
      glm::vec3 v3( i_vertices[i3 + 0], i_vertices[i3 + 1], i_vertices[i3 + 2] );

      glm::vec3 v1tov2 = v2-v1;
      glm::vec3 v1tov3 = v3-v1;

      glm::vec3 n = glm::normalize(glm::cross(v1tov2, v1tov3));

      o_normals[i1 + 0] = n.x;
      o_normals[i1 + 1] = n.y;
      o_normals[i1 + 2] = n.z;
      o_normals[i1 + 3] = 0.0f;

      o_normals[i2 + 0] = n.x;
      o_normals[i2 + 1] = n.y;
      o_normals[i2 + 2] = n.z;
      o_normals[i2 + 3] = 0.0f;

      o_normals[i3 + 0] = n.x;
      o_normals[i3 + 1] = n.y;
      o_normals[i3 + 2] = n.z;
      o_normals[i3 + 3] = 0.0f;
    }
  }
  
  Abstract_geometric_object::~Abstract_geometric_object()
  {
    if ( m_vbo_id[0] > 0 )
    {
      glDeleteBuffers(to_integral(VBO_type::Count), m_vbo_id);
    }
  }
  
  void Abstract_geometric_object::initialize_data_structures(Shader_program *i_shader)
  {
    glGenBuffers(to_integral(VBO_type::Count), m_vbo_id);
    
    internal_initialize_data_structures(i_shader);
    
    calculate_model_matrix();
  }

  void Abstract_geometric_object::draw(Shader_program *i_shader)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_type::Position)]);
    GLint vertex_attrib_location = i_shader->get_attribute_location(in_position);
    glEnableVertexAttribArray(vertex_attrib_location);
    glVertexAttribPointer(vertex_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_type::Normal)]);
    GLint normals_attrib_location = i_shader->get_attribute_location(in_normal);
    glEnableVertexAttribArray(normals_attrib_location);
    glVertexAttribPointer(normals_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind our third VBO as being the active buffer and storing vertex array indicies
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_type::Index)]);

    glDrawElements(GL_TRIANGLES, m_triangle_count, GL_UNSIGNED_INT, 0);

    // unbind & disable
    glDisableVertexAttribArray(vertex_attrib_location);
    glDisableVertexAttribArray(normals_attrib_location);
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  }
  
  void Abstract_geometric_object::calculate_model_matrix()
  {
    glm::mat4 translation = glm::translate(glm::mat4(1), m_position);
    glm::mat4 scale = glm::scale(m_scale);
    m_model_matrix = translation * scale;
  }


} // namespace mkay