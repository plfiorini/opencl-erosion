#include <erosion/include/Module_erosion.h>
#include <common/include/Logger.h>
#include <gfx/include/Ressource_manager.h>

#include <CL/cl.hpp>

using namespace std;
namespace po = boost::program_options;

namespace mkay
{ 
  Module_erosion::Module_erosion()
    : m_window_manager{}
    , m_input_manager{}
    , m_cl_manager{}
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
    m_window_manager.create("Erosion", make_tuple(c_default_width, c_default_height));
    
    //m_cl_manager = CL_manager{};
    //m_cl_manager.init(CL_DEVICE_TYPE_ALL, "AMD Accelerated Parallel Processing");
    
    m_input_manager = SDL_input_manager{};
    
    m_skybox.init();

    loginf << "finished configuration" << endl;
  }
  
  void Module_erosion::step()
  {
    m_input_manager.handle_event_loop();
    m_input_manager.update_camera(m_camera);

    m_skybox.render(m_camera);
    
    //loginf << "step" << endl;
    m_window_manager.swap_buffers();
  }
  
  void Module_erosion::shutdown()
  {
    m_cl_manager.shutdown(); 
    m_window_manager.destroy();
    
    loginf << "finished shutdown" << endl;
  }
}