project(input)

include(build_functions)

add_standard_lib(
  common
)

use_third_party_cmake(libsdl2)
use_opengl()