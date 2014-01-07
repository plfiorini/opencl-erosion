#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <common/include/glm_definitions.h>

#define ASPECT_RATIO(width, height) \
  static_cast<double>(width)/static_cast<double>(height)

#define ROTATION_SCALE  0.1f
#define ZOOM_SCALE      2.0f

namespace mkay
{
  class Camera
  {
  public:
    Camera();
    
    void setup_scene(bool i_no_translation = false);
    void recalculate_projection_matrix();
    
    void set_home( glm::vec3 const & i_position
                 , glm::vec2 const & i_rotation ) 
    { 
      m_home_position = i_position; 
      m_home_rotation = i_rotation;
    }
    void return_home();
    
    void set_aspect_ratio(int i_width, int i_height) 
    { 
      m_aspect_ratio = ASPECT_RATIO(i_width, i_height);
    }
    void set_aspect_ratio(double i_aspect_ratio) { m_aspect_ratio = i_aspect_ratio; }
    double get_aspect_ratio() { return m_aspect_ratio; }
    
    glm::mat4 get_projection_view_matrix() { return m_projection*m_view; }
    
    void move(glm::vec3 const & i_translation);
    void set_position(glm::vec3 const &i_position) { m_virtual_position = i_position; m_position = -i_position; }
    glm::vec3 const & get_position() { return m_virtual_position; }
    
    void rotate(glm::vec2 const & i_rotation);
    void set_rotation(glm::vec2 const & i_rotation) { m_rotation = m_virtual_rotation = glm::vec3{}; rotate(i_rotation, true); }
    glm::vec3 const & get_rotation() { return m_virtual_rotation; }
    
    void print(std::ostream & outstr) const;
    
  private:
    glm::vec3 m_home_position;
    glm::vec2 m_home_rotation;

    glm::vec3 m_virtual_position = glm::vec3(0.0f, 100.0f, 0.0f);
    glm::vec3 m_position = -m_virtual_position;
    
    glm::vec3 m_translation = glm::vec3{0.0f};
    
    glm::vec3 m_virtual_rotation = glm::vec3{0.0f};
    glm::vec3 m_rotation = -m_virtual_rotation;
    glm::vec3 m_rotation_axis_x = X_AXIS;
    
    glm::vec3 m_zoom = glm::vec3{0.0f};
    
    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_inv_view;
    
    double m_aspect_ratio = 1.0;
    double m_FOV = 60.0;
    double m_near_clipping_plane = 0.01;
    double m_far_clipping_plane = 10000.0;
    
    void rotate(glm::vec2 const & i_rotation, bool i_ignore_gimbal);
  };
} // namespace mkay

std::ostream &operator << (std::ostream & o_outstr, const mkay::Camera & i_camera);

#endif // __CAMERA_H__