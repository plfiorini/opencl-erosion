project(cl)

include(build_functions)

add_standard_lib(
  common
)

use_third_party_cmake(glm)
use_opengl()
use_opencl()
