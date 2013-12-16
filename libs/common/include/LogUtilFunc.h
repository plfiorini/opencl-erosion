#pragma once
#ifndef __LOGUTILFUNC_H__
#define __LOGUTILFUNC_H__

#include <glm/glm.hpp>

#include <iostream>

std::ostream &operator<<(std::ostream &o, const glm::vec2 &vec);
std::ostream &operator<<(std::ostream &o, const glm::vec3 &vec);
std::ostream &operator<<(std::ostream &o, const glm::vec4 &vec);

#endif // __LOGUTILFUNC_H__