project(erosion)

include(build_functions)

add_standard_module(
  module
  gfx
  input
  cl
)

use_shader(
  skybox
)

use_boost(
  program_options
)