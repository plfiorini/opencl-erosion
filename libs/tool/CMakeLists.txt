project(tool)

include(build_functions)

add_standard_lib(
  common
)

use_system_library(GL)
use_third_party_cmake(libsdl2)
use_opencl()