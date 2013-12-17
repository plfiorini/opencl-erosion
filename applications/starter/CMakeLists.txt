project(starter)

include(build_functions)

add_standard_executable(
  common
)

use_third_party_cmake(libsdl2)
use_system_library(GL)