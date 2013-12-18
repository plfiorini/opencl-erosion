#include <tool/include/SDL_window_manager.h>
#include <tool/include/SDL_exception.h>
#include <common/include/Logger.h>

#include <sstream>

#ifdef WIN32
	#include <Windows.h>
#endif
#include <GL/gl.h>

using namespace std;

#define CHECK_SDL_ERROR() check_SDL_error(__LINE__)
#define CHECK_SDL_ERROR_THROW() check_SDL_error(__LINE__, true)

namespace mkay
{
  void check_SDL_error(int line = -1, bool throw_ex = false)
  {
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
      stringstream os;
      os << "SDL Error: " << error;
      if (line != -1)
        os << " on line " << line;
      if ( !throw_ex )
      {
        logerr << os.str() << endl;    
        SDL_ClearError();
      }
      else
      {
        BOOST_THROW_EXCEPTION(
          SDL_exception()
            << errinfo_str(os.str())
        );
      }
    }
  }
  
  SDL_window_manager::SDL_window_manager(
    string const & i_window_name, 
    dimension32_t const & i_window_size
  )
    : m_window(nullptr)
    , m_context()
    , m_window_name(i_window_name)
    , m_window_size(i_window_size)
  {
  }
  
  SDL_window_manager::~SDL_window_manager()
  {
    loginf << "destructor" << endl;
    
    destroy();
  }
  
  void SDL_window_manager::create()
  {
    if ( m_window )
    {
      logwarn << "window already created ..." << endl;
      return;
    }
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      BOOST_THROW_EXCEPTION(
        SDL_exception()
          << errinfo_cstr("unable to initialize SDL")
      );
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // turn on double buffering 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // and 24bit Z buffer.
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // create window
    m_window = SDL_CreateWindow( 
      m_window_name.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      get<0>(m_window_size), get<1>(m_window_size),
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN 
    );
    if ( !m_window )
    {
      BOOST_THROW_EXCEPTION(
        SDL_exception()
          << errinfo_cstr("unable to create window")
      );
    }
    CHECK_SDL_ERROR_THROW();
    
    // create opengl context
    m_context = SDL_GL_CreateContext(m_window);
    CHECK_SDL_ERROR_THROW();
    
    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);
    CHECK_SDL_ERROR();
    
    // clear screen
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    this->swap_buffers();
    
    int major, minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    loginf << "created context: OpenGL " << major << "." << minor << endl;
    
    // profiles are mutually exclusive
    int profile;
    stringstream os;
    os << "profile: ";
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
    switch(profile)
    {
      case SDL_GL_CONTEXT_PROFILE_CORE: os << "core"; break;
      case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY: os << "compatibility"; break;
      case SDL_GL_CONTEXT_PROFILE_ES: os << "ES"; break;
      default: os << "unknown(" << profile << ")"; break;
    }
    loginf << os.str() << endl;
    
    loginf << "finished init" << endl;
  }
  
  void SDL_window_manager::destroy()
  {
    loginf << "destroy" << endl;

    if ( !m_window )
    {
      logwarn << "window not existing!" << endl;
      return;
    }
    
    // delete opengl context & destroy window
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);    
    //SDL_Quit();
    logwarn << "FIXME: SDL_Quit not working" << endl;
    m_window = nullptr;
  }
}