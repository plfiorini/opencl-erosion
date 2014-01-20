#pragma once
#ifndef __SDL_FONT_MANAGER_H__
#define __SDL_FONT_MANAGER_H__

#include <string>

#include <SDL2/SDL_ttf.h>

namespace mkay
{
  class SDL_font_manager
  {
  public:
    void init(std::string const & i_default_font);
  };
} // namespace mkay

#endif // __SDL_FONT_MANAGER_H__