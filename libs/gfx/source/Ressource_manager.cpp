#include <gfx/include/Ressource_manager.h>
#include <gfx/include/Texture_cube_map.h>
#include <gfx/include/Ressource_exception.h>
#include <common/include/Logger.h>

#include <IL/ilut.h>

#include <memory>

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
      object->add_source(vertex_path, GL_VERTEX_SHADER);
      object->add_source(fragment_path, GL_FRAGMENT_SHADER);      
      if ( !geometry_path.empty() )
      {
        object->add_source(geometry_path, GL_GEOMETRY_SHADER);
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
  
  std::string Ressource_manager::lookup_path(std::string const &i_name)
  {
    for(std::string const & path_prefix : m_search_paths)
    {
      std::string full_path = path_prefix + "/" + i_name;
      logdeb << "Testing path: " << full_path << endl;
      
      path p(full_path);
      if (exists(p))
      {
        return full_path;
      }
    }
    BOOST_THROW_EXCEPTION(
      Ressource_exception()
        << errinfo_str("Ressource not found in lookup paths: " + i_name)
    );
  }
  
} // namespace mkay