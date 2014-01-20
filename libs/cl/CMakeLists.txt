project(cl)

include(build_functions)

add_standard_lib(
  common
)

use_third_party(glm)
use_opengl()
use_opencl()
