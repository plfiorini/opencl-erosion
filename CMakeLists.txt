# set project name
#set(MAIN_PROJECT_NAME "opencl-erosion" CACHE INTERNAL "the solution name" FORCE)
project(${MAIN_PROJECT_NAME})
#add_custom_target(${MAIN_PROJECT_NAME})

# CMake requests a specification of the minimum version
cmake_minimum_required(VERSION 2.8)

# include CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake/modules)

# include default cache variables
include(cache)

# include compiler info
include(compiler)

# include third party lib functions
include(third_party)

# include build functions
include(build_functions)

# include the versioning module to generate version headers
include(versioning)
generate_version_info()

# Extend include path for all projects
include_directories(
  SYSTEM /usr/include
  SYSTEM ${CMAKE_BINARY_DIR}/output/third_party/include  
  ${CMAKE_HOME_DIRECTORY}/libs
  ${CMAKE_HOME_DIRECTORY}/modules
  ${CMAKE_HOME_DIRECTORY}/shaders
  ${CMAKE_BINARY_DIR}
)

# include subdirs
add_subdirectory(third_party)
add_subdirectory(libs)
add_subdirectory(modules)
add_subdirectory(applications)
add_subdirectory(shaders)
