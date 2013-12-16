# set project name
project(erosion)

# CMake requests a specification of the minimum version
cmake_minimum_required(VERSION 2.8)

# include CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake/modules)

# include the versioning module to generate version headers
include(versioning)
generate_version_info()

# Extend include path for all libraries
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

# include subdirs
add_subdirectory(applications)
add_subdirectory(libs)
