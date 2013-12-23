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
      ("width", po::value<int>(), "window width")
      ("height", po::value<int>(), "window height")
      ("ressource_path", po::value<string>()->required(), "path to ressources")
    ;
  }
  
  void Module_erosion::configure(boost::program_options::variables_map const & i_variables)
  {
    // recreate all managers
    
    std::string base_search_path = i_variables["ressource_path"].as<string>();
    Ressource_manager::instance().init(base_search_path);
    
    m_window_manager = SDL_window_manager{};
    m_window_manager.create("Erosion", make_tuple(1280, 768));
    
    //m_cl_manager = CL_manager{};
    //m_cl_manager.init(CL_DEVICE_TYPE_ALL, "AMD Accelerated Parallel Processing");
    
    m_input_manager = SDL_input_manager{};
    
    m_skybox.init();

    loginf << "finished configuration" << endl;
  }
  
  void Module_erosion::step()
  {
    m_input_manager.handle_event_loop();

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