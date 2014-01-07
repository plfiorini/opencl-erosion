#include <gfx/include/Camera.h>
#include <common/include/Logger.h>

namespace mkay
{
  Camera::Camera()
  {
  }
  
  void Camera::setup_scene(bool i_no_translation /* = false */)
  {
    glm::mat4 mat_zoom = glm::translate(glm::mat4{1.0f}, m_zoom);  
    glm::mat4 inv_mat_zoom = glm::translate(glm::mat4{1.0f}, -m_zoom);  

    glm::mat4 rotate_x, rotate_y, inv_rotate_x, inv_rotate_y;
    // yaw => left/right around the y-axis
    rotate_y = glm::rotate(rotate_y, m_rotation.y, Y_AXIS);
    inv_rotate_y = glm::rotate(inv_rotate_y, -m_rotation.y, Y_AXIS);
    // pitch => up/down therefor around the x-axis
    rotate_x = glm::rotate(rotate_x, m_rotation.x, m_rotation_axis_x);
    inv_rotate_x = glm::rotate(inv_rotate_x, -m_rotation.x, m_rotation_axis_x);

    //glm::mat4 matTranslation = 
    //  glm::gtc::matrix_transform::translate(glm::mat4(1.0f), m_Position);  

    // zoom out, rotate around y, rotate around x
    //m_View = matTranslation * rotateX * rotateY * matZoom;
    //m_View = matZoom * rotateY * rotateX;

    if( !i_no_translation )
    {
      double frameTime = 1.0; //GLUTProgram::Instance()->GetFrameTime();
      m_virtual_position -= m_translation * static_cast<float>(frameTime);
      m_position += m_translation * static_cast<float>(frameTime);

      glm::mat4 mat_translate = glm::translate(glm::mat4(1.0f), m_position);  
      glm::mat4 inv_mat_translate = glm::translate(glm::mat4(1.0f), m_virtual_position);  

      m_view = mat_zoom * rotate_y * rotate_x * mat_translate;
      m_inv_view = inv_mat_zoom * inv_rotate_y * inv_rotate_x * inv_mat_translate;
    }
    else
    {
      m_view = rotate_y * rotate_x;
      m_inv_view = inv_rotate_y * inv_rotate_x;
    }
  }
  
  void Camera::recalculate_projection_matrix()
  {
    // setup new projection matrix
    m_projection = glm::perspective( m_FOV
                                   , m_aspect_ratio
                                   , m_near_clipping_plane
                                   , m_far_clipping_plane);
  }
  
  void Camera::return_home()
  {
    set_position(m_home_position); 
    set_rotation(m_home_rotation);
    m_zoom = glm::vec3(0.0f);
  }
  
  void Camera::move(glm::vec3 const & i_translation)
  {
    glm::mat4 inv_rotate_x, inv_rotate_y;

    inv_rotate_y = glm::rotate(inv_rotate_y, -m_rotation.y, Y_AXIS);
    inv_rotate_x = glm::rotate(inv_rotate_x, -m_rotation.x, m_rotation_axis_x);

    m_translation = glm::vec3(inv_rotate_x * inv_rotate_y * glm::vec4(i_translation, 1.0));
  }

  void Camera::rotate(const glm::vec2 & i_rotation)
  {
    rotate(i_rotation, false);
  }
  
  void Camera::print(std::ostream & outstr) const
  {
    outstr << "Camera Position: " << m_virtual_position;
    outstr << " Pitch " << m_virtual_rotation.x;
    outstr << " Yaw " << m_virtual_rotation.y;
    outstr << " Zoom " << m_zoom.z;
  }
  
  void Camera::rotate(glm::vec2 const & i_rotation, bool i_ignore_gimbal)
  {
    m_virtual_rotation.xy += i_rotation;
    
    // x = pitch => up/down therefor around the x-axis
    // y = yaw => left/right around the y-axis
    m_rotation.xy += i_rotation * ROTATION_SCALE;

    // prevent gimbal lock
    if( !i_ignore_gimbal )
    {
      if( m_rotation.x > 80.0f )
      {
        m_rotation.x = 80.0f;
      }
      else if( m_rotation.x < -80.0f )
      {
        m_rotation.x = -80.0f;
      }
    }

    // calculate the new x rotation axis
    // basically take the original x axis and rotate it around y axis by rotation.y
    glm::mat4 mat_rot = glm::rotate(glm::mat4(1.0f), m_rotation.y, Y_AXIS);
    m_rotation_axis_x = glm::vec3( glm::vec4( X_AXIS, 1.0f ) * mat_rot );
  }
} // namespace mkay


std::ostream &operator << (std::ostream & o_outstr, const mkay::Camera & i_camera)
{
  i_camera.print(o_outstr);
  return o_outstr;
}