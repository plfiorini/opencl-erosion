#include <tool/include/CL_manager.h>
#include <tool/include/CL_exception.h>
#include <common/include/Logger.h>

#include <vector>

#include <boost/algorithm/string.hpp>

#ifndef WIN32
	#include <GL/glx.h>
#endif

using std::vector;
using std::string;
using std::endl;

#define CL_CHECK(error_code, msg)                       \
  if ( CL_SUCCESS != error_code )                       \
  {                                                     \
    BOOST_THROW_EXCEPTION(                              \
      CL_exception()                                    \
        << errinfo_str(msg)                           \
        << errinfo_cstr(get_error_string(error_code))   \
    );                                                  \
  }

namespace mkay
{
  const char * get_error_string(const cl_int i_Error)
  {
    switch (i_Error) 
    {
      case CL_SUCCESS:                          return "Success!"; 
      case CL_DEVICE_NOT_FOUND:                 return "Device not found."; 
      case CL_DEVICE_NOT_AVAILABLE:             return "Device not available"; 
      case CL_COMPILER_NOT_AVAILABLE:           return "Compiler not available"; 
      case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return "Memory object allocation failure"; 
      case CL_OUT_OF_RESOURCES:                 return "Out of resources"; 
      case CL_OUT_OF_HOST_MEMORY:               return "Out of host memory"; 
      case CL_PROFILING_INFO_NOT_AVAILABLE:     return "Profiling information not available"; 
      case CL_MEM_COPY_OVERLAP:                 return "Memory copy overlap"; 
      case CL_IMAGE_FORMAT_MISMATCH:            return "Image format mismatch"; 
      case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return "Image format not supported"; 
      case CL_BUILD_PROGRAM_FAILURE:            return "Program build failure"; 
      case CL_MAP_FAILURE:                      return "Map failure"; 
      case CL_INVALID_VALUE:                    return "Invalid value"; 
      case CL_INVALID_DEVICE_TYPE:              return "Invalid device type"; 
      case CL_INVALID_PLATFORM:                 return "Invalid platform"; 
      case CL_INVALID_DEVICE:                   return "Invalid device"; 
      case CL_INVALID_CONTEXT:                  return "Invalid context"; 
      case CL_INVALID_QUEUE_PROPERTIES:         return "Invalid queue properties"; 
      case CL_INVALID_COMMAND_QUEUE:            return "Invalid command queue"; 
      case CL_INVALID_HOST_PTR:                 return "Invalid host pointer"; 
      case CL_INVALID_MEM_OBJECT:               return "Invalid memory object"; 
      case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return "Invalid image format descriptor"; 
      case CL_INVALID_IMAGE_SIZE:               return "Invalid image size"; 
      case CL_INVALID_SAMPLER:                  return "Invalid sampler"; 
      case CL_INVALID_BINARY:                   return "Invalid binary"; 
      case CL_INVALID_BUILD_OPTIONS:            return "Invalid build options"; 
      case CL_INVALID_PROGRAM:                  return "Invalid program"; 
      case CL_INVALID_PROGRAM_EXECUTABLE:       return "Invalid program executable"; 
      case CL_INVALID_KERNEL_NAME:              return "Invalid kernel name"; 
      case CL_INVALID_KERNEL_DEFINITION:        return "Invalid kernel definition"; 
      case CL_INVALID_KERNEL:                   return "Invalid kernel"; 
      case CL_INVALID_ARG_INDEX:                return "Invalid argument index"; 
      case CL_INVALID_ARG_VALUE:                return "Invalid argument value"; 
      case CL_INVALID_ARG_SIZE:                 return "Invalid argument size"; 
      case CL_INVALID_KERNEL_ARGS:              return "Invalid kernel arguments"; 
      case CL_INVALID_WORK_DIMENSION:           return "Invalid work dimension"; 
      case CL_INVALID_WORK_GROUP_SIZE:          return "Invalid work group size"; 
      case CL_INVALID_WORK_ITEM_SIZE:           return "Invalid work item size"; 
      case CL_INVALID_GLOBAL_OFFSET:            return "Invalid global offset"; 
      case CL_INVALID_EVENT_WAIT_LIST:          return "Invalid event wait list"; 
      case CL_INVALID_EVENT:                    return "Invalid event"; 
      case CL_INVALID_OPERATION:                return "Invalid operation"; 
      case CL_INVALID_GL_OBJECT:                return "Invalid OpenGL object"; 
      case CL_INVALID_BUFFER_SIZE:              return "Invalid buffer size"; 
      case CL_INVALID_MIP_LEVEL:                return "Invalid mip-map level"; 
      default:                                  return "Unknown Error"; 
    }
  }
  
  void print_platform_info(cl::Platform const & i_platform)
  {
    std::string platformVendor;
    i_platform.getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
    std::string platformName;
    i_platform.getInfo((cl_platform_info)CL_PLATFORM_NAME, &platformName);
    std::string platformProfile;
    i_platform.getInfo((cl_platform_info)CL_PLATFORM_PROFILE, &platformProfile);
    std::string platformVersion;
    i_platform.getInfo((cl_platform_info)CL_PLATFORM_VERSION, &platformVersion);
    std::string platformExt;
    i_platform.getInfo((cl_platform_info)CL_PLATFORM_EXTENSIONS, &platformExt);

    loginf << "Platform Info: " << platformVendor << endl;
    loginf << "* name: '" << platformName << "'" << endl;
    loginf << "* profile: " << platformProfile << endl;    
    loginf << "* version: " << platformVersion << endl;    
    loginf << "* extensions: " << platformExt << endl;    
  }
  
  cl::Platform choose_platform(std::string const & i_platform_name)
  {
    // get all platforms
    vector<cl::Platform> platforms;
    cl_int error = cl::Platform::get(&platforms);
    CL_CHECK( error, "could not get platforms" );
    
    if( platforms.size() < 1 )
    {
      BOOST_THROW_EXCEPTION(
        CL_exception{}
          << errinfo_cstr{"no suitable platform found - exiting!"}
      );
    }
    
    loginf << "available platforms: " << platforms.size() << endl;
    loginf << "--------------------------------------------------" << endl;
    // print out info
    for(size_t i=0; i<platforms.size(); ++i)
    {
      print_platform_info( platforms[i] );
    }
    loginf << "--------------------------------------------------" << endl;
    
    // platform selection
    cl::Platform choosen_platform;
    if( !i_platform_name.empty() )
    {
      for (size_t i=0; i<platforms.size(); ++i)
      {
        std::string platform_name;
        platforms[i].getInfo((cl_platform_info)CL_PLATFORM_NAME, &platform_name);

        // both tolower
        std::string compare = platform_name;
        boost::algorithm::to_lower(compare);

        std::string wanted = i_platform_name;
        boost::algorithm::to_lower(wanted);

        if( compare.find(wanted) != string::npos )
        {
          loginf << "choosing platform: " << platform_name << endl;
          return platforms[i];
        }
      }
 
      logwarn << "could not get platform: " << i_platform_name << endl;
    }

    // fallback
    loginf << "choosing default platform 0" << endl;
    return platforms[0];
  }
    
  void print_device_info(cl::Device &i_Device)
  {
    std::string deviceName;
    i_Device.getInfo(CL_DEVICE_NAME, &deviceName);
    std::string deviceVendor;
    i_Device.getInfo(CL_DEVICE_VENDOR, &deviceVendor);
    std::string deviceProfile;
    i_Device.getInfo(CL_DEVICE_PROFILE, &deviceProfile);
    std::string deviceVersion;
    i_Device.getInfo(CL_DEVICE_VERSION, &deviceVersion);
    std::string deviceDriverVersion;
    i_Device.getInfo(CL_DRIVER_VERSION, &deviceDriverVersion);
    cl_uint deviceComputeUnits;
    i_Device.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &deviceComputeUnits); 
    // official nvidia opencl implementation does not support this string -> app crash
    //std::string deviceOpenCLVersion;
    //i_Device.getInfo(CL_DEVICE_OPENCL_C_VERSION, &deviceOpenCLVersion);
    std::string deviceExtensions;
    i_Device.getInfo(CL_DEVICE_EXTENSIONS, &deviceExtensions);

    loginf << "Device Info: " << deviceVendor << " - " << deviceName << std::endl;
    loginf << "* profile: " << deviceProfile << std::endl;    
    loginf << "* version: " << deviceVersion << std::endl;    
    loginf << "* driver version: " << deviceDriverVersion << std::endl;    
    loginf << "* compute units: " << deviceComputeUnits << std::endl;
    //loginf << "* opencl c version: " << deviceOpenCLVersion << std::endl;
    loginf << "* extensions: " << deviceExtensions << std::endl;
    
    loginf << "* Max work items dimensions: "  
          << i_Device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() 
          << std::endl;

    std::vector< ::size_t> deviceMaxWorkItemSizes;
    i_Device.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &deviceMaxWorkItemSizes);

    for(cl_uint i=0; i<i_Device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>(); ++i)
    {
      loginf << "\t  - Max work items[" << i << "]: " << deviceMaxWorkItemSizes[i] << std::endl;  
    }
  }
  
  cl::Device choose_device(cl::Platform &i_platform, cl_device_type i_device_type)
  {
    // now get all available devices with type i_DeviceType from the choosen platform
    std::vector<cl::Device> all_devices;
    cl_int error = i_platform.getDevices(i_device_type, &all_devices);
    CL_CHECK( error, "could not get device list from platform" );

    // if no devices where found return initialization failed (=false)
    if( all_devices.size() < 1 )
    {
      BOOST_THROW_EXCEPTION(
        CL_exception()
          << errinfo_cstr("no suitable devices found - exiting!")
      );
    }

    loginf << "available devices for choosen platform: " << all_devices.size() << endl;  
    loginf << "--------------------------------------------------" << endl;
    for( size_t i=0; i<all_devices.size(); ++i )
    {
      print_device_info( all_devices[i] );
    }
    loginf << "--------------------------------------------------" << endl;
    
    loginf << "using device 0" << endl;
    return all_devices[0];
  }

  void CL_manager::init( 
    cl_device_type i_device_type /* = CL_DEVICE_TYPE_GPU */,
    std::string const & i_platform_name /*= std::string() */
  )
  {
    loginf << "initializing OpenCL ..." << endl;

    cl::Platform choosen_platform = choose_platform(i_platform_name);
    std::string platform_name;
    choosen_platform.getInfo((cl_platform_info)CL_PLATFORM_NAME, &platform_name);
    loginf << "selected platform: " << platform_name << endl;

    cl::Device choosen_device = choose_device(choosen_platform, i_device_type);
    std::string device_name;
    choosen_device.getInfo((cl_platform_info)CL_DEVICE_NAME, &device_name);
    loginf << "selected device: " << device_name << endl;

    // the next step is to get the current opengl context
    // and create an common opencl context from it
#ifdef WIN32
    cl_context_properties contextProperties[] =
    {
      CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
      CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
      CL_CONTEXT_PLATFORM, (cl_context_properties)(choosen_platform)(),
      0 // property terminator
  };
#else
    cl_context_properties contextProperties[] =
    {
      CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
      CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
      CL_CONTEXT_PLATFORM, (cl_context_properties)(choosen_platform)(),
      0 // property terminator
    };
#endif
    
    // prepare vector for the devices we want to use
    m_used_devices.clear();
    m_used_devices.push_back(choosen_device);
    
    // create context with the coosen devices and properties
    cl_int error;
    m_context = cl::Context(m_used_devices, contextProperties, NULL, NULL, &error);
    CL_CHECK( error, "could not create context" );

    // and now that we have a context, create a command queue
    cl_command_queue_properties cqProperties = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
    m_command_queue = cl::CommandQueue(m_context, choosen_device, cqProperties, &error);
    CL_CHECK( error, "could not create commandqueue" );

    loginf << "created commandqueue successfully!" << endl;
    
  }
  
  void CL_manager::shutdown()
  {
  }
  

} // namespace mkay