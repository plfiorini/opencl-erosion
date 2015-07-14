#define _USE_MATH_DEFINES
#include <cmath>

#include <gl/glew.h>

#include <erosion/include/Module_erosion.h>
#include <common/include/Logger.h>

#include <gfx/include/Ressource_manager.h>
#include <gfx/include/Shader_program.h>
#include <gfx/include/Material_factory.h>
#include <gfx/include/Colored_material.h>

#include <common_shader/include/vars.hgl>
#include <color_shader/include/vars.hgl>

using namespace std;
namespace po = boost::program_options;

namespace mkay
{ 
  Module_erosion::Module_erosion()
    : m_window_manager{}
    , m_event_manager{}
    , m_cl_manager{}
    , m_camera()
    , m_light()
    , m_skybox()
    , m_geo_object(100.0f)
    , m_light_object(10.0f)
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
    
    m_cl_manager = CL_manager{};
    m_cl_manager.init(CL_DEVICE_TYPE_ALL, "AMD Accelerated Parallel Processing");
    
    m_event_manager = SDL_event_manager{};
    m_event_manager.configure(&m_window_manager, &m_camera);
    
    m_camera.set_home( glm::vec3{0.0f, 615.0f, 200.0f}
                     , glm::vec2{720.0f, 0.0f} );
    m_camera.return_home();
    m_camera.set_aspect_ratio(ASPECT_RATIO(c_default_width, c_default_height));
    m_camera.recalculate_projection_matrix();
    
    m_skybox.init();
    
    // initialize own rendering stuff
    Colored_material_ptr red_material(
      dynamic_cast<Colored_material *>(
        Material_factory::create(Material_type::Colored)
      )
    );
    red_material->set_material_color(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
    m_geo_object.buffer_data(red_material);
    m_geo_object.set_position(glm::vec3{0.0f});
    m_geo_object.calculate_model_matrix();
    
    m_light.set_position(glm::vec3{0.0f});
    m_light.set_ambient_color(glm::vec4{0.2f, 0.2f, 0.2f, 1.0f});
    m_light.set_diffuse_color(glm::vec4{1.0f});
    m_light.set_specular_color(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    m_light.set_range(5000.0f);
    m_light.set_linear_attenuation(0.2f);
    m_light.set_quadratic_attenuation(0.01f);
    
    Colored_material_ptr white_material(
      dynamic_cast<Colored_material *>(
        Material_factory::create(Material_type::Colored)
      )
    );
    white_material->ignore_lighting(true);
    white_material->set_material_color(glm::vec4{1.0f});
    m_light_object.buffer_data(white_material);
    m_light_object.set_position(m_light.get_position());
    m_light_object.calculate_model_matrix();

    loginf << "finished configuration" << endl;
  }
  
  void Module_erosion::step()
  {
    m_laptime.take_time();
    update_scene();    
    render_scene();
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
    m_skybox.render(m_camera);
    
    // draw own stuff    
    m_camera.setup_scene();
    
    glEnable(GL_DEPTH_TEST);
    
    {
      m_geo_object.get_shader()->use();
      m_light.let_there_be_light(m_geo_object.get_shader());
      m_geo_object.draw(m_camera);
    }
    {
      m_light_object.get_shader()->use();
      m_light.let_there_be_light(m_light_object.get_shader());
      m_light_object.draw(m_camera);
    }
  }
  
  void Module_erosion::update_scene()
  {
    //loginf << "laptime: " << m_laptime.get_microseconds().count() << " µs" << endl;
    
    m_event_manager.handle_event_loop();
    
    static float x=0.0f;
    x+=0.05f;
    if( x>2.0f*M_PI ) x=0.0f;    
    
    glm::vec3 pos = m_light.get_position();
    
    const float scale = 500.0f;
    pos.x = scale*sin(x);
    pos.y = scale*sin(x)*cos(x);
    pos.z = scale*cos(x);
    
//     loginf << "lightpos = " << pos << endl;
    
    m_light.set_position(pos);
    m_light_object.set_position(pos);
    m_light_object.calculate_model_matrix();
    
//     loginf << "camerapos = " << m_camera.get_position() << endl;
  }

}