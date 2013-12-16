#pragma once
#ifndef __LOG_UTIL_FUNC_H__
#define __LOG_UTIL_FUNC_H__

#include <glm/glm.hpp>

#include <iostream>

namespace mkay
{
  std::ostream &operator<<(std::ostream &o, const glm::vec2 &vec);
  std::ostream &operator<<(std::ostream &o, const glm::vec3 &vec);
  std::ostream &operator<<(std::ostream &o, const glm::vec4 &vec);
} // namespace mkay

#endif // __LOG_UTIL_FUNC_H__