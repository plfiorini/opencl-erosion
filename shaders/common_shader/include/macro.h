#pragma once
#ifndef __MACRO_H__
#define __MACRO_H__

#include <common/include/Macros.h>

/// definition macro for shader variables
#define SHADER_VAR(io, type, name) \
  const char *name = STRINGIFY(name)
  
#endif // __MACRO_H__