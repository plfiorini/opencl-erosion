#pragma once
#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <type_traits>

namespace mkay
{
  template<typename E>
  constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type 
  {
    return static_cast<typename std::underlying_type<E>::type>(e);
  }
} // namespace mkay

#endif // __TOOLS_H__