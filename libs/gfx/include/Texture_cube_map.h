#pragma once
#ifndef __TEXTURE_CUBE_MAP_H__
#define __TEXTURE_CUBE_MAP_H__

#include <string>

namespace mkay
{
  class Texture_cube_map
  {
  protected:
    friend class Ressource_manager;
    
    void load_from(std::string const &i_path);
    
  public:
    Texture_cube_map() = default;
  };
} // namespace mkay

#endif __TEXTURE_CUBE_MAP_H__