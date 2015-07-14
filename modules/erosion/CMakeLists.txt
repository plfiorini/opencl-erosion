project(erosion)

include(build_functions)

add_standard_module(
  module
  gfx
  cl
)

use_opencl()
use_opengl()
use_devil()
use_third_party(glm)
use_sdl()
use_sdlttf()
use_boost(
  filesystem 
  program_options
  system
)