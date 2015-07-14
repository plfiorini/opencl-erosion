#pragma once
#ifndef __MODULE_H__
#define __MODULE_H__

#include <boost/program_options.hpp>
#include <memory>

namespace mkay
{
  /// \brief Base class for all Modules
  class Module
  {
  public:
    virtual void get_options_description(boost::program_options::options_description & o_description) = 0;    
    virtual void configure(boost::program_options::variables_map const & i_variables) = 0;
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
  
  /// \brief smart pointer typedef for Module
  typedef std::shared_ptr<Module> Module_ptr_t;  
} // namespace mkay

#endif // __MODULE_H__