#include <iostream>
#include <sstream>

#include <unistd.h>
#include <signal.h>

#include <common/include/Mkay_exception.h>
#include <common/include/Logger.h>

#include <erosion/include/Module_erosion.h>

#include <generated/include/version_gen_fwd.hpp>

#include <GL/gl.h>
#include <SDL2/SDL.h>

using namespace mkay;
using namespace std;

static volatile bool exit_flag = false;

void signal_handler(int signum) 
{
  std::stringstream os;
  os << "received signal: ";
  switch (signum) 
  {
    case SIGHUP:
      os << "SIGHUP";
      break;
    case SIGINT:
      os << "SIGINT";
      exit_flag = true;
      break;
    case SIGTERM:
      os << "SIGTERM";
      exit_flag = true;
      break;
    case SIGQUIT:
      os << "SIGQUIT";
      exit_flag = true;
      break;
    case SIGUSR1:
      os << "SIGUSR1";
      break;
    case SIGPIPE:
      os << "SIGPIPE";
      break;
    case SIGALRM:
      os << "SIGALRM";
      break;
    case SIGCHLD:
      os << "SIGCHLD";
      break;
    case SIGXFSZ:
      os << "SIGCHLD";
      break;
    default:
      break;
  }
  logdeb << os.str() << std::endl;
}

/*
void init_sdl()
{
  // Clear our buffer with a red background
  glClearColor ( 1.0, 0.0, 0.0, 1.0 );
  glClear ( GL_COLOR_BUFFER_BIT );
  // Swap our back buffer to the front
  SDL_GL_SwapWindow(mainwindow);
  // Wait 2 seconds
  SDL_Delay(2000);
}
*/

int main(void)
{
  signal(SIGHUP,  signal_handler);
  signal(SIGINT,  signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGUSR1, signal_handler);
  signal(SIGPIPE, signal_handler);
  signal(SIGALRM, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGCHLD, signal_handler);
  signal(SIGXFSZ, signal_handler);
  
  Logger::init_defaults();
  Logger::set_verbosity(Log_level::Debug);
  
  loginf << "starting up ..." << endl;
  
  loginf << "Version information following" << std::endl 
         << "Compiled on: " << COMPILATION_DATE << std::endl
         << Version_info::get_complete_version_info();

  try
  {
    Module_erosion *program = new Module_erosion();

    loginf << "configuring module" << std::endl;
    program->configure();
    
    loginf << "starting execution" << std::endl;
    while ( false == program->requests_exit() && false == exit_flag )
    {
      program->step();
      usleep(1000);
    }
    
    loginf << "commencing shutdown" << std::endl;
    program->shutdown();
  }
  catch (Mkay_exception const & ex)
  {
    logerr << boost::diagnostic_information(ex);
  }
  
  loginf << "end of line" << std::endl;
}
