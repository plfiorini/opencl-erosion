#include <gfx/include/SDL_window_manager.h>
#include <gfx/include/SDL_exception.h>
#include <common/include/Logger.h>

#include <sstream>
#include <list>

#ifdef WIN32
	#include <Windows.h>
#endif
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

#define CHECK_SDL_ERROR() check_SDL_error(__LINE__)
#define CHECK_SDL_ERROR_THROW() check_SDL_error(__LINE__, true)

namespace mkay
{
  void gl_error_callback( 
    GLenum source, GLenum type, GLuint id, GLuint severity,
    GLsizei length, const char *message, void *user_parameter )
  {
    std::stringstream stringStream;
    std::string sourceString;
    std::string typeString;
    std::string severityString;
 
    switch (source) 
    {
    case GL_DEBUG_SOURCE_API_ARB: 
      sourceString = "API";
      break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB: 
      sourceString = "Application";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: 
      sourceString = "Window System";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: 
      sourceString = "Shader Compiler";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: 
      sourceString = "Third Party";
      break;
    case GL_DEBUG_SOURCE_OTHER_ARB: 
      sourceString = "Other";
      break;
    default: 
      sourceString = "Unknown";
      break;
    }
 
    switch (type) 
    {
      case GL_DEBUG_TYPE_ERROR_ARB:
        typeString = "Error";
        break;
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: 
        typeString = "Deprecated Behavior";
        break;
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        typeString = "Undefined Behavior";
        break;
      case GL_DEBUG_TYPE_PORTABILITY_ARB:
        typeString = "Portability";
        break;
      case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        typeString = "Performance";
        break;
      case GL_DEBUG_TYPE_OTHER_ARB:
        typeString = "Other";
        break;
      default:
        typeString = "Unknown";
        break;
    }
 
    switch (severity) 
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:
      severityString = "High";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
      severityString = "Medium";
      break;
    case GL_DEBUG_SEVERITY_LOW_ARB:
      severityString = "Low";
      break;
    default:
      severityString = "Unknown";
      break;
    }
 
    stringStream << "OpenGL Error: " << message;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";
 
    logerr << "throwing error ..." << endl;
    
    BOOST_THROW_EXCEPTION(
      SDL_exception()
        << errinfo_str(stringStream.str())
    );
  }
  
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
    destroy();
  }
  
  void SDL_window_manager::create(
    string const & i_window_name, 
    glm::ivec2 const & i_window_size
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
    
    // list of opengl versions to try
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
                
        // SDL_GL_CONTEXT_PROFILE_CORE
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        CHECK_SDL_ERROR();
        
        // create window
        m_window = SDL_CreateWindow( 
          m_window_name.c_str(),
          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
          m_window_size.x, m_window_size.y,
          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
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

        // initialize glew for extension handling
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
          BOOST_THROW_EXCEPTION(
            SDL_exception()
              << errinfo_cstr("glew could not be initialized")
          );
        }
        
        // provide dummy parameters for glutInit
        int argc = 1;
        char *argv[] = { static_cast<char*>("empty") };
        glutInit(&argc, argv);
        
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
    
    // antialiasing
    glEnable(GL_MULTISAMPLE);
    
    // clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    this->swap_buffers();
    
    // z-buffer
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // backface culling
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK);
    
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
    
    glDebugMessageCallbackARB(gl_error_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    
    loginf << "finished init" << endl;
  }
  
  void SDL_window_manager::destroy()
  {
    loginf << "cleanup ..." << endl;

    if ( !m_window )
    {
      logwarn << "was not initialized - skipping cleanup!" << endl;
      return;
    }
    
    // delete opengl context & destroy window
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);    
    //SDL_Quit();
    logwarn << "FIXME: SDL_Quit not working" << endl;
    m_window = nullptr;
  }
  
  void SDL_window_manager::on_resize(glm::ivec2 const & i_new_window_size)
  {
    m_window_size = i_new_window_size;
    glViewport( 0, 0
              , m_window_size.x, m_window_size.y );
  }

}