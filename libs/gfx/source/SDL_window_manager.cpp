#include <gfx/include/SDL_window_manager.h>
#include <gfx/include/SDL_exception.h>
#include <common/include/Logger.h>

#include <sstream>
#include <list>

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
      
      SDL_ClearError();
      if ( !throw_ex )
      {
        logerr << os.str() << endl;
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
  
  SDL_window_manager::SDL_window_manager()
    : m_window(nullptr)
    , m_context()
    , m_window_name()
    , m_window_size()
  {
  }
  
  SDL_window_manager::~SDL_window_manager()
  {
    loginf << "destructor" << endl;
    
    destroy();
  }
  
  void SDL_window_manager::create(
    string const & i_window_name, 
    dimension32_t const & i_window_size
  )
  {
    if ( m_window )
    {
      logwarn << "window already created ..." << endl;
      return;
    }
    
    m_window_name = i_window_name;
    m_window_size = i_window_size;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      BOOST_THROW_EXCEPTION(
        SDL_exception()
          << errinfo_cstr("unable to initialize SDL")
      );
    }
    
    std::list<std::pair<int,int>> gl_version
    { 
      {4,3}, {4,2},
      {3,3}, {3,2},
      {2,1}
    };
    bool config_ok = false;
    for ( auto current_version = gl_version.begin();
          current_version != gl_version.end() && config_ok == false;
          ++current_version )
    {     
      try
      {
        loginf << "trying to create window with opengl context " << current_version->first << "." << current_version->second << endl;
        
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, current_version->first);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, current_version->second);
        CHECK_SDL_ERROR();
        
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1); 
        CHECK_SDL_ERROR();

        // turn on double buffering 
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        // and 24bit Z buffer.
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        CHECK_SDL_ERROR();

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
        
        config_ok = true;
      }
      catch(SDL_exception const & ex)
      {
        logerr << boost::diagnostic_information(ex);
      }
    }
    
    if ( !config_ok )
    {
      BOOST_THROW_EXCEPTION(
        SDL_exception()
          << errinfo_cstr("no compatible opengl context found")
      );
    }
    
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