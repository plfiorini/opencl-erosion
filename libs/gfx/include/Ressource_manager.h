#ifndef __RESSOURCE_MANAGER_H_
#define __RESSOURCE_MANAGER_H_

#include <list>
#include <map>

#include <common/include/Singleton.h>
#include <gfx/include/Texture_cube_map.h>

namespace mkay
{
  enum class Ressource_type
  {
    Texture,
    Texture_cube_map
  };
  
  class Ressource_manager
    : public Singleton<Ressource_manager>
  {
  protected:
    Ressource_manager() = default;
    friend class Singleton<Ressource_manager>;
    
  public:    
    void init(std::list<std::string> const & i_search_path);
  
    template<typename T, Ressource_type>
    T & get(std::string const & i_name);
    
  private:
    std::list<std::string> & m_search_path;
    
    std::map<std::string, Texture_cube_map> m_cube_maps;
    
    std::string lookup_path(std::string const &i_name);
  };

} // namespace mkay

#endif // __RESSOURCE_MANAGER_H_