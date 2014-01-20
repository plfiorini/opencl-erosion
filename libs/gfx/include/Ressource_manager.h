#ifndef __RESSOURCE_MANAGER_H_
#define __RESSOURCE_MANAGER_H_

#include <list>
#include <map>

#include <common/include/Singleton.h>

#include <gfx/include/Shader_program.h>
#include <gfx/include/Texture.h>
#include <gfx/include/Texture_cube_map.h>

#include <SDL2/SDL_ttf.h>

#define RESMAN \
  Ressource_manager::instance()
  
namespace mkay
{
  class Ressource_manager
    : public Singleton<Ressource_manager>
  {
  protected:
    Ressource_manager() = default;
    friend class Singleton<Ressource_manager>;
    
  public:    
    void init(std::string i_base_path = "", std::list<std::string> const & i_search_paths = std::list<std::string>{});
  
    template<typename T>
    T * get(std::string const & i_name);
    
  private:
    std::list<std::string> m_search_paths;
    
    std::map<std::string, Texture *> m_textures;
    std::map<std::string, Texture_cube_map *> m_cube_maps;
    std::map<std::string, Shader_program *> m_shaders;
    std::map<std::string, TTF_Font*> m_fonts;
    
    template<typename X>
    X * get_internal( std::map<std::string, X *> & i_ressources,
                      std::string const & i_name );
    
    std::string lookup_path(std::string const &i_name);
    
    std::string load_shader_source(std::string const & i_path);
    std::string load_shader_include(std::string const & i_path);
  };
    
  template<typename X>
  X * Ressource_manager::get_internal(
    std::map<std::string, X *> & i_ressources,
    std::string const & i_name
  )
  {
    auto it = i_ressources.find(i_name);
    if ( it == i_ressources.end() )
    {
      std::string full_path = lookup_path(i_name);      
      auto * object = new X();
      object->load_from(full_path);
      i_ressources[i_name] = object;
      return object;
    }
    else
    {
      return it->second;
    }
  }
} // namespace mkay

#endif // __RESSOURCE_MANAGER_H_