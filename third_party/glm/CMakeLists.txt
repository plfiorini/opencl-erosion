project(glm)

include(build_functions)

add_cmake_third_party(
  NAME "glm-0.9.4.6.zip"
  VERSION "0.9.4.6"
  PARAMETERS -DGLM_TEST_ENABLE:BOOL=ON
)