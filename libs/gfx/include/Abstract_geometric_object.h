#pragma once
#ifndef __ABSTRACT_GEOMETRIC_OBJECT_H__
#define __ABSTRACT_GEOMETRIC_OBJECT_H__

#include <common/include/Tools.h>
#include <common/include/glm_definitions.h>

#include <gfx/include/Abstract_material.h>

#include <GL/glew.h>

namespace mkay
{
  class Camera;
  
  enum class VBO_type
    : GLuint
  {
    Position = 0,
    Normal = 1,
    Index = 2,
    Count
  };
  
  void calculate_normals(
    const float * i_vertices, const uint32_t i_vertex_count,
    const uint32_t *i_indices, const uint32_t i_index_count,
    float *o_normals, bool i_invert = false
  );
  
  class Abstract_geometric_object
  {
  public:
    Abstract_geometric_object() = default;
    virtual ~Abstract_geometric_object() = 0;
    
    void buffer_data(Abstract_material_ptr i_material);
    
    void draw(Camera const &i_camera);
    
    void calculate_model_matrix();
    glm::mat4 const & get_model_matrix() { return m_model_matrix; }
    
    void set_position(glm::vec3 const & i_position) { m_position = i_position; }
    glm::vec3 const & get_position() { return m_position; }
    
    void set_uniform_scale(float i_scale) { m_scale = glm::vec3{i_scale}; }
    void set_scale(glm::vec3 const & i_scale) { m_scale = i_scale; }
    glm::vec3 const & get_scale() { return m_scale; }
    void reset_scale() { m_scale = glm::vec3{1.0f}; }
    
    void set_material(Abstract_material_ptr i_material);
    Abstract_material_ptr get_material() { return m_material; }
    Shader_program * get_shader() { return m_material->get_shader(); }
    
  protected:   
		GLuint m_vbo_id[to_integral(VBO_type::Count)];
    GLsizei m_triangle_count = 0;
    
    glm::mat4 m_model_matrix;
    glm::vec3 m_position = glm::vec3{0.0f};
    glm::vec3 m_scale = glm::vec3{1.0f};
    
    Abstract_material_ptr m_material;
    
    virtual void internal_buffer_data(Abstract_material_ptr i_material) = 0;    
  };
} // namespace mkay

#endif // __ABSTRACT_GEOMETRIC_OBJECT_H__