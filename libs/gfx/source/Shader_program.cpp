#include <fstream>

#include <gfx/include/Shader_program.h>
#include <gfx/include/Shader_exception.h>
#include <common/include/Logger.h>

using namespace std;

namespace mkay
{
  const char * get_shader_type_name(GLenum i_shader_type)
  {
    switch( i_shader_type )
    {
    case GL_VERTEX_SHADER: return "Vertex";
    case GL_FRAGMENT_SHADER: return "Fragment";
    case GL_GEOMETRY_SHADER: return "Geometry";
    default:
      BOOST_THROW_EXCEPTION(
        Shader_exception{}
          << errinfo_cstr{"Unknown Shader Type"}
          << errinfo_size_t{i_shader_type}
      );
    }
  }
  
  void print_build_info_log(GLuint i_object_id, bool is_program)
  {
    GLint infolog_length = 0;
    GLint chars_written  = 0;
    char *info_log = nullptr;
    glGetShaderiv(i_object_id, GL_INFO_LOG_LENGTH, &infolog_length);
    if (infolog_length > 0)
    {
      info_log = (char *)malloc(infolog_length);
      if ( is_program )
      {
        glGetProgramInfoLog(i_object_id, infolog_length, &chars_written, info_log);
      }
      else
      {
        glGetShaderInfoLog(i_object_id, infolog_length, &chars_written, info_log);
      }
      loginf << info_log;
      free(info_log);
    }
    else
    {
      loginf << "Shader Compilation: OK" << endl;
    }
  }
  
  void print_attrib_info(GLuint i_program_id)
  {
    char *name = nullptr;
    GLint active_attribs, max_length;

    glGetProgramiv(i_program_id, GL_ACTIVE_ATTRIBUTES, &active_attribs);
    glGetProgramiv(i_program_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

    name = (char *)malloc(max_length + 1);

    for (GLint i = 0; i < active_attribs; i++) 
    {
      GLint size;
      GLenum type;

      glGetActiveAttrib( i_program_id, i, max_length + 1, NULL
                       , &size, &type, name);
      
      logdeb << type << " " << name << " is at location " 
             << glGetAttribLocation(i_program_id, name) 
             << endl;
    }

    free(name);
  }
  
  void Shader_program::add_source(std::string const & i_path, GLenum i_type)
  {
    loginf << "adding source for " 
           << get_shader_type_name(i_type)
           << " from " << i_path << endl;
    
    ifstream file(i_path);
    if( !file.is_open() )
    {
      BOOST_THROW_EXCEPTION(
        Shader_exception{}
          << errinfo_str{std::string{"Could not open file: "} + i_path}
      );
    }
    
    // get source which is already stored
    std::string & source = m_shader_source[i_type];
    
    std::string line;
    while (std::getline(file, line))
    {
      source += line + "\n";
    }
    
    file.close();
  }
  
  void Shader_program::build(std::string const & i_name)
  {
    if ( m_program_id )
    {
      logwarn << "program built already: " << m_program_id << endl;
    }
  
    m_name = i_name;
  
    // compile all shader types
    for ( auto & kv : m_shader_source )
    {
      compile_shader(kv.first, kv.second);
    }
    
    // clear shader source map, we don't need it anymore
    m_shader_source.clear();
    
    // links shaders to program
    link_program();
    
    glUseProgram(0);
    
    loginf << "shader program " << m_name << " finished building!" << endl;
  }
  
  GLint Shader_program::get_uniform_location(const char *i_name)
  {
    return glGetUniformLocation(m_program_id, i_name);
  }
  
  void Shader_program::compile_shader(GLenum i_type, std::string & i_source)
  {
    loginf << "compiling shader: " << get_shader_type_name(i_type) << endl;
    
    GLuint shader_id = glCreateShader(i_type);
    
    logdeb << "shader code: " << endl
           << i_source << endl;
    
    const char *source_ptr = i_source.c_str();
    glShaderSource(shader_id, 1, static_cast<const GLchar**>(&source_ptr), 0);
    glCompileShader(shader_id);
    
    loginf << "compilation finished, getting compiler log" << endl;
    
    // check status of compilation
    GLint status = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    print_build_info_log(shader_id, false);
    
    if ( GL_TRUE != status )
    {
      BOOST_THROW_EXCEPTION(
        Shader_exception{}
          << errinfo_cstr{"shader compilation failed"}
      );
    }
    
    m_shader_ids.push_back(shader_id);
  }
  
  void Shader_program::link_program()
  {
    loginf << "linking shader program: " << m_name << endl;
    
    m_program_id = glCreateProgram();
    if ( !m_program_id )
    {
      BOOST_THROW_EXCEPTION(
        Shader_exception{}
          << errinfo_cstr{"could not create shader program object"}
      );
    }
    
    // attach all shaders to program object
    for( GLuint shader_id : m_shader_ids ) 
    {
      glAttachShader(m_program_id, shader_id);
    }
    
    glLinkProgram(m_program_id);
    print_build_info_log(m_program_id, true);
    
    // check status of linking
    GLint status = 0;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &status);
    if (GL_TRUE != status) 
    {
      BOOST_THROW_EXCEPTION(
        Shader_exception{}
          << errinfo_cstr{"shader program linking failed"}
      );
    }
  }
} // namespace mkay