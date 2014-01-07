#include <erosion/include/Module_erosion.h>
#include <common/include/Logger.h>

#include <gfx/include/Ressource_manager.h>
#include <gfx/include/Shader_program.h>

#include <CL/cl.hpp>

#include <common_shader/include/vars.hgl>
#include <standard/include/vars.hgl>

using namespace std;
namespace po = boost::program_options;

namespace mkay
{ 
  Module_erosion::Module_erosion()
    : m_window_manager{}
    , m_event_manager{}
    , m_cl_manager{}
    , m_shader(nullptr)
    , m_camera()
    , m_light()
    , m_skybox()
    , m_geo_object(100.0f)
  {
  }
  
  Module_erosion::~Module_erosion()
  {
  }
  
  void Module_erosion::get_options_description(po::options_description & o_description)
  {
    o_description.add_options()
      ("width", po::value<int>()->default_value(c_default_width), "window width")
      ("height", po::value<int>()->default_value(c_default_height), "window height")
      ("ressource_path", po::value<string>()->default_value(c_default_search_path), "path to ressources")
    ;
  }
  
  void Module_erosion::configure(boost::program_options::variables_map const & i_variables)
  {
    // recreate all managers
    
    std::string base_search_path = i_variables["ressource_path"].as<string>();
    Ressource_manager::instance().init(base_search_path);
    
    m_window_manager = SDL_window_manager{};
    m_window_manager.create("Erosion", glm::ivec2{c_default_width, c_default_height});
    
    //m_cl_manager = CL_manager{};
    //m_cl_manager.init(CL_DEVICE_TYPE_ALL, "AMD Accelerated Parallel Processing");
    
    m_event_manager = SDL_event_manager{};
    m_event_manager.configure(&m_window_manager, &m_camera);
    
    m_camera.set_home( glm::vec3{0.0f, 615.0f, 200.0f}
                     , glm::vec2{720.0f, 0.0f} );
    m_camera.return_home();
    m_camera.set_aspect_ratio(ASPECT_RATIO(c_default_width, c_default_height));
    m_camera.recalculate_projection_matrix();
    
    m_skybox.init();
    
    // initialize own rendering stuff
    
    m_shader = RESMAN.get<Shader_program>("standard");
    
    m_geo_object.initialize_data_structures(m_shader);
    m_geo_object.set_position(glm::vec3{0.0f});

    loginf << "finished configuration" << endl;
  }
  
  void Module_erosion::step()
  {
    m_event_manager.handle_event_loop();

    render_scene();
    
    //loginf << "step" << endl;
    m_window_manager.swap_buffers();
  }
  
  void Module_erosion::shutdown()
  {
    m_cl_manager.shutdown(); 
    m_window_manager.destroy();
    
    loginf << "finished shutdown" << endl;
  }
  
  void Module_erosion::render_scene()
  {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // draw skybox
    //m_skybox.render(m_camera);
    
    // draw own stuff
    m_geo_object.calculate_model_matrix();
    
    m_shader->use();
    
    m_camera.setup_scene();
    
    m_light.let_there_be_light(m_shader);
    
    glEnable(GL_DEPTH_TEST);
    
    glm::mat4 const & model = m_geo_object.get_model_matrix();
    glm::mat4 mvp = m_camera.get_projection_view_matrix() * model;
    glUniformMatrix4fv(m_shader->get_uniform_location(model_view_projection), 1, GL_FALSE, glm::value_ptr(mvp) );
    glUniformMatrix4fv(m_shader->get_uniform_location(model_matrix), 1, GL_FALSE, glm::value_ptr(model) );

    m_geo_object.draw(m_shader);
    
    m_shader->disable();
  }

}