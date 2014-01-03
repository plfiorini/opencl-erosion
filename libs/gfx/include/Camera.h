#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <common/include/glm_definitions.h>

namespace mkay
{
  class Camera
  {
  public:
    Camera();
    
    void setup_scene(bool i_no_translation = false);
    
    glm::mat4 get_projection_view_matrix() { return m_projection*m_view; }
    
    void move(glm::vec3 i_translation);
    
  private:
    
    glm::vec3 m_virtual_position = glm::vec3(0.0f, 100.0f, 0.0f);
    glm::vec3 m_position = -m_virtual_position;
    
    glm::vec3 m_translation = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glm::vec3 m_virtual_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_rotation = -m_virtual_rotation;
    glm::vec3 m_rotation_axis_x = X_AXIS;
    
    glm::vec3 m_zoom = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_inv_view;
  };
} // namespace mkay

#endif // __CAMERA_H__