#pragma once
#ifndef __TIME_MEASUREMENT_H__
#define __TIME_MEASUREMENT_H__

#include <chrono>

namespace mkay
{
  class Time_measurement
  {
  public:
    Time_measurement() = default;
    
    void take_time();
    
    std::chrono::microseconds get_microseconds() const { return m_round_time; } 
   
  private:
    std::chrono::high_resolution_clock::time_point m_last = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds m_round_time = std::chrono::microseconds{0};
  };
} // namespace mkay

#endif // __TIME_MEASUREMENT_H__