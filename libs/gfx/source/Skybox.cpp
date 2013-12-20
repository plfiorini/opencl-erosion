#include <gfx/include/Skybox.h>
#include <gfx/include/Camera.h>
#include <gfx/include/Ressource_manager.h>

#include <skybox/include/vars.h>

using namespace std;

namespace mkay
{
  void Skybox::init()
  {
    m_shader = RESMAN.get<Shader_program>("skybox");
    m_texture = RESMAN.get<Texture_cube_map>("cubemap_brightday1");
  }
  
  void Skybox::render(Camera & i_camera)
  {
    // setup camera without translation
    i_camera.setup_scene(true);
    
    glm::mat4 const MVP = i_camera.get_projection_view_matrix();
    
    m_shader->use();
    
    glUniformMatrix4fv(m_shader->get_uniform_location(model_view_projection), 1, GL_FALSE, glm::value_ptr(MVP) );
    
    
  }
} // namespace mkay