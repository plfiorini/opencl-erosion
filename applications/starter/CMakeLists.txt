project(starter)
reset_project()

include(build_functions)

add_standard_executable(
  common
  gfx
  erosion
  platform
)

use_opencl()
use_devil()
use_sdl()
use_sdlttf()

use_third_party(glm)

use_boost(
  filesystem 
  program_options
)

use_opengl()