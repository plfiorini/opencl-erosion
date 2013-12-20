#include <gfx/include/Ressource_manager.h>
#include <gfx/include/Texture_cube_map.h>

#include <IL/ilut.h>

#include <memory>

namespace mkay
{
  void Ressource_manager::init(std::list<std::string> const & i_search_paths)
  {
    m_search_paths = i_search_paths;
    m_search_paths.push_back("../textures");
    
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
      std::string vertex_path = lookup_path("shaders/"+i_name+"/vertex.glsl");
      std::string fragment_path = lookup_path("shaders/"+i_name+"/fragment.glsl");
      std::string geometry_path = lookup_path("shaders/"+i_name+"/geometry.glsl");

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
    return "../"+i_name;
  }
  
} // namespace mkay