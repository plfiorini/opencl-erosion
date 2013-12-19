#include <erosion/include/Module_erosion.h>
#include <common/include/Logger.h>

using namespace std;

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
  
  void Module_erosion::configure()
  {
    // recreate all managers
    
    m_window_manager = SDL_window_manager{};
    m_window_manager.create("Erosion", make_tuple(1280, 768));
    
    m_cl_manager = CL_manager{};
    m_cl_manager.init();
    
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