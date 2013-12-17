#pragma once
#ifndef __MODULE_H__
#define __MODULE_H__

#include <boost/noncopyable.hpp>

namespace mkay
{
  class Module
    : public boost::noncopyable
  {
  public:
    virtual void configure() = 0;
    virtual void step() = 0;
    virtual void shutdown() = 0;
    virtual bool requests_exit() = 0;
  };
} // namespace mkay

#endif // __MODULE_H__