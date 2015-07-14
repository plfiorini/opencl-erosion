#pragma once
#ifndef __CL_MANAGER_H__
#define __CL_MANAGER_H__

//#define __CL_ENABLE_EXCEPTIONS
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.hpp>

namespace mkay
{
  class CL_manager
  {
  public:
    CL_manager() = default;
    
    void init(
      cl_device_type i_device_type = CL_DEVICE_TYPE_GPU,
      std::string const & i_platform_name = std::string()
    );
    
    void shutdown();
    
  private:
    cl::Context m_context;
    cl::CommandQueue m_command_queue;
    std::vector<cl::Device> m_used_devices;
  };
} // namespace mkay

#endif // __CL_MANAGER_H__