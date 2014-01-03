project(input)

include(build_functions)

add_standard_lib(
  common
  gfx
)

use_third_party_cmake(libsdl2)
use_opengl()