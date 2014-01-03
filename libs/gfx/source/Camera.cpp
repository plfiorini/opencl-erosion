#include <gfx/include/Camera.h>

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
  
  void Camera::move(glm::vec3 i_translation)
  {
    glm::mat4 invRotateX, invRotateY;

    invRotateY = glm::rotate(invRotateY, -m_rotation.y, Y_AXIS);
    invRotateX = glm::rotate(invRotateX, -m_rotation.x, m_rotation_axis_x);

    m_translation = glm::vec3(invRotateX * invRotateY * glm::vec4(i_translation, 1.0));
  }

} // namespace mkay