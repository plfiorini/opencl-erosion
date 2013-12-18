#include <erosion/include/Module_erosion.h>
#include <common/include/Logger.h>

using namespace std;

namespace mkay
{
  Module_erosion::Module_erosion()
    : m_window_manager{}
  {
  }
  
  Module_erosion::~Module_erosion()
  {
  }
  
  void Module_erosion::configure()
  {
    m_window_manager = unique_ptr<SDL_window_manager>{
      new SDL_window_manager{"Erosion", make_tuple(1280, 768)}
    };
    m_window_manager->create();
    
    m_cl_manager = unique_ptr<CL_manager>{
      new CL_manager{}
    };
    m_cl_manager->init();

    loginf << "finished configuration" << endl;
  }
  
  void Module_erosion::step()
  {
    //loginf << "step" << endl;
    m_window_manager->swap_buffers();
  }
  
  void Module_erosion::shutdown()
  {
    if ( m_cl_manager )
    {
      m_cl_manager->shutdown();
      m_cl_manager.reset();
    }
     
    if ( m_window_manager )
    {
      m_window_manager->destroy();
      m_window_manager.reset();
    }
    
    loginf << "finished shutdown" << endl;
  }
}