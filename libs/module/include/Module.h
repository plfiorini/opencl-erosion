#pragma once
#ifndef __MODULE_H__
#define __MODULE_H__

namespace mkay
{
  class Module
  {
  public:
    virtual void configure() = 0;
    virtual void step() = 0;
    virtual void shutdown() = 0;
    virtual bool requests_exit() = 0;
    
    virtual ~Module() = 0;
    
  protected:
    Module() = default;
    
  private:
    Module(const Module &) = delete;
    Module & operator=(const Module &) = delete;
  };
} // namespace mkay

#endif // __MODULE_H__