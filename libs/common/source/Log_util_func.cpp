#include <common/include/Log_util_func.h>

//----------------------------------------------------------------------
std::ostream &operator<<(std::ostream &o, const glm::vec2 &vec)
{
  o << vec.x << "/" << vec.y;
  return o;
}

//----------------------------------------------------------------------
std::ostream &operator<<(std::ostream &o, const glm::vec3 &vec)
{
  o << vec.x << "/" << vec.y << "/" << vec.z;
  return o;
}

//----------------------------------------------------------------------
std::ostream &operator<<(std::ostream &o, const glm::vec4 &vec)
{
  o << vec.x << "/" << vec.y << "/" << vec.z << "/" << vec.w;
  return o;
}