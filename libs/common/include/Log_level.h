#pragma once
#ifndef __LOG_LEVEL_H__
#define __LOG_LEVEL_H__

namespace mkay
{
  enum class Log_level
    : std::int8_t
  {
    Fatal,
    Error,
    Warning,
    Info,
    Detail,
    Debug,
    Log_level_count
  };
} // namespace mkay
  
#endif // __LOG_LEVEL_H__