#pragma once
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>
#include <memory>

namespace mkay
{
  template <class T>
  class Singleton
  {
  public:
    template <typename... Args>
    static
    T& instance(Args... args)
    {
      static std::unique_ptr<T> s_instance;
      if (!s_instance)
      {
        s_instance = std::unique_ptr<T>(
          new T(std::forward<Args>(args)...)
        );
      }

      return *s_instance;
    }
    
  private:
    Singleton(Singleton const &) = delete;
    Singleton & operator=(Singleton const &) = delete;    
  };
} // namespace mkay

#endif // __SINGLETON_H__