project(erosion)

include(build_functions)

add_standard_module(
  module
  gfx
  cl
)

use_boost(
  program_options
)