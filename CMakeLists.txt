# set project name
project(erosion)

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

# Extend include path for all libraries
include_directories(
  ${CMAKE_HOME_DIRECTORY}/libs
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/output/third_party/include
)

# include subdirs
add_subdirectory(third_party)
add_subdirectory(libs)
add_subdirectory(modules)
add_subdirectory(applications)