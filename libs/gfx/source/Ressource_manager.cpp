#include <gfx/include/Ressource_manager.h>

#include <gfx/include/Texture_cube_map.h>

namespace mkay
{
  void Ressource_manager::init(std::list<std::string> const & i_search_path)
  {
    m_search_path = i_search_path;
  }
    
  template<>
  Texture_cube_map & Ressource_manager::get<Texture_cube_map, Ressource_type::Texture>(
    std::string const & i_name
  )
  {
    auto it = m_cube_maps.find(i_name);
    if ( it != m_cube_maps.end() )
    { 
      return it->second;
    }
    else
    {
      auto & cm = m_cube_maps[i_name];
      std::string path = lookup_path(i_name);
      cm.load_from(path);
      return cm;
    }
  }
  
  std::string Ressource_manager::lookup_path(std::string const &i_name)
  {
    return i_name;
  }
  
} // namespace mkay