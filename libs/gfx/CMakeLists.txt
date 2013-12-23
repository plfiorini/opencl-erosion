project(gfx)

include(build_functions)

add_standard_lib(
  common
)

use_opengl()
use_devil()
use_third_party_cmake(libsdl2)
use_boost(filesystem)