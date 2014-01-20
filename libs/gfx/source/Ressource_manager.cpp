#include <gfx/include/Ressource_manager.h>
#include <gfx/include/Texture_cube_map.h>
#include <gfx/include/Ressource_exception.h>
#include <common/include/Logger.h>

#include <IL/ilut.h>

#include <memory>
#include <fstream>
#include <algorithm>

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

namespace mkay
{
  void Ressource_manager::init(std::string i_base_path /* = "" */, std::list<std::string> const & i_search_paths /* = std::list<std::string>{} */)
  {    
    loginf << "current path: " << boost::filesystem::current_path() << endl;

    m_search_paths = i_search_paths;
    for ( std::string & path : m_search_paths )
    {
      path = i_base_path + "/" + path;
    }
    m_search_paths.push_back(i_base_path + "/textures");
    m_search_paths.push_back(i_base_path + "/shaders");
    
    // init DevIL
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
    
    // init ttf lib
    if (TTF_Init() == -1)
    {
      BOOST_THROW_EXCEPTION(
        Ressource_exception{}
          << errinfo_str{std::string{"could not initialize sdl ttf library: "} + TTF_GetError()}
      );
    }
  }

  template<>
  Texture * Ressource_manager::get<Texture>(
    std::string const & i_name
  )
  {
     return get_internal(m_textures, i_name);
  }
  
  template<>
  Texture_cube_map * Ressource_manager::get<Texture_cube_map>(
    std::string const & i_name
  )
  {
    return get_internal(m_cube_maps, i_name);
  }
  
  template<>
  Shader_program * Ressource_manager::get<Shader_program>(
    std::string const & i_name
  )
  {
    auto it = m_shaders.find(i_name);
    if ( it == m_shaders.end() )
    {
      std::string vertex_path = lookup_path(i_name+"/source/vertex.glsl");
      std::string fragment_path = lookup_path(i_name+"/source/fragment.glsl");
      // geometry is optional
      std::string geometry_path;
      try 
      {
        geometry_path = lookup_path(i_name+"/source/geometry.glsl");
      }
      catch(Ressource_exception const & ex)
      {
        logwarn << "no geometry shader found for: " << i_name << " - skipping!" << endl;
      }

      auto object = new Shader_program();
      object->set_source(GL_VERTEX_SHADER, load_shader_source(vertex_path) );
      object->set_source(GL_FRAGMENT_SHADER, load_shader_source(fragment_path) );
      if ( !geometry_path.empty() )
      {
        object->set_source(GL_GEOMETRY_SHADER, load_shader_source(geometry_path) );
      }
      object->build(i_name);
      m_shaders[i_name] = object;
      return object;
    }
    else
    {
      return it->second;
    }
  }
  
  template<>
  TTF_Font * Ressource_manager::get<TTF_Font>(
      std::string const & i_name
  )
  {
    auto it = m_fonts.find(i_name);
    if ( it == m_fonts.end() )
    {
      std::string full_path = lookup_path(i_name);      
      auto * object = TTF_OpenFont("images\\segoepr.ttf",52);
      m_fonts[i_name] = object;
      return object;
    }
    else
    {
      return it->second;
    }
  }
  
  std::string Ressource_manager::lookup_path(std::string const &i_name)
  {
    for(std::string const & path_prefix : m_search_paths)
    {
      std::string full_path = path_prefix + "/" + i_name;
      logdeb << "testing path: " << full_path << endl;
      
      path p(full_path);
      if (exists(p))
      {
        return full_path;
      }
    }
    BOOST_THROW_EXCEPTION(
      Ressource_exception{}
        << errinfo_str{"ressource not found in lookup paths: " + i_name}
    );
  }
  
  string Ressource_manager::load_shader_source(const string& i_path)
  {
    ifstream file(i_path);
    if( !file.is_open() )
    {
      BOOST_THROW_EXCEPTION(
        Ressource_exception{}
          << errinfo_str{std::string{"could not open file: "} + i_path}
      );
    }
      
    std::string source;
    std::string line;
    while (std::getline(file, line))
    {
      size_t pos = line.find("#include");
      if ( pos != string::npos )
      {
        size_t begin = line.find("<");
        size_t end = line.rfind(">");
        if ( begin == string::npos || end == string::npos )
        {
          BOOST_THROW_EXCEPTION(
            Ressource_exception{}
              << errinfo_str{"syntax error in file: " + i_path + " - line: " + line}
          );
        }
        std::string file_name = line.substr(begin+1, (end-begin-1));
        std::string include_path = lookup_path(file_name);
        logdeb << "including " << include_path << endl;
        
        source += load_shader_include(include_path);
      }
      else
      {
        source += line + "\n";
      }      
    }
    
    file.close();
    
    return source;
  }

  std::string Ressource_manager::load_shader_include(std::string const & i_path)
  {
    std::ifstream include_file(i_path);
    if( !include_file.is_open() )
    {
      BOOST_THROW_EXCEPTION(
        Ressource_exception{}
          << errinfo_str{"could not open include file: " + i_path}            
      );
    }
    
    std::stringstream source;
    std::string line;
    while (std::getline(include_file, line))
    {
      size_t pos = line.find("SHADER_VAR(");
      if ( pos != string::npos )
      {
        size_t begin = line.find("(");
        size_t end = line.find(")");
        if ( begin == string::npos || end == string::npos )
        {
          BOOST_THROW_EXCEPTION(
            Ressource_exception{}
              << errinfo_str{"syntax error in file: " + i_path + " - line: " + line}
          );
        }
        
        std::string varline = line.substr(begin+1, (end-begin-1));
        std::replace( varline.begin(), varline.end(),
                      ',', ' ' );
        source << varline + ";" << endl;
      }
      else
      {
        source << line << endl;
      }
    }
    
    include_file.close();
    
    return source.str();
  }
  
} // namespace mkay