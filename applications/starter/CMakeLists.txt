project(starter)

include(build_functions)

add_standard_executable(
  common
  erosion
)

use_boost(
  program_options
)
