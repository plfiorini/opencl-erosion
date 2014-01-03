include(CMakeParseArguments)
include(ExternalProject)

include(versioning)
include(installation)

#
# Check that the given library_type is valid.
# Valid types are "STATIC" and "SHARED"
#
function(assert_library_type library_type)
  string(TOUPPER ${library_type} library_type)
  if((NOT ${library_type} STREQUAL "STATIC") AND
     (NOT ${library_type} STREQUAL "SHARED"))
     log_fatal_error("Unknown library_type (${library_type}) specified.")
  endif()
endfunction(assert_library_type)

# 
# add the namespace prefix to every library name
#
function(private_add_project_libraries target_name)
  set(link_libraries "")
  foreach(ll ${INSTALL_UNPARSED_ARGUMENTS})
    log_debug("Adding lib ${ll}")
    list(APPEND link_libraries "${ll}")

    # look out for header dependencies of this lib
    log_debug("Testing var ${ll}${PROJECT_DEPENDENCY_POSTFIX}")
    set(header_deps "")
    if(${ll}${PROJECT_DEPENDENCY_POSTFIX})
      log_debug("Content: ${${ll}${PROJECT_DEPENDENCY_POSTFIX}}")
      foreach(hd ${${ll}${PROJECT_DEPENDENCY_POSTFIX}})
        log_debug("Adding include path ${hd}")

        # also add include path to my depencenies
        add_include_dependency(${hd})
      endforeach()
    endif()
  endforeach()

  target_link_libraries(${target_name} 
    ${link_libraries}
  )
endfunction(private_add_project_libraries)

function(private_add_project_headers)
  foreach(ll ${INSTALL_UNPARSED_ARGUMENTS})
    # look out for header dependencies of this lib
    log_debug("Testing var ${ll}${PROJECT_DEPENDENCY_POSTFIX}")
    if(${ll}${PROJECT_DEPENDENCY_POSTFIX})
      log_debug("Content: ${${ll}${PROJECT_DEPENDENCY_POSTFIX}}")
      foreach(hd ${${ll}${PROJECT_DEPENDENCY_POSTFIX}})
        log_debug("Adding include path ${hd}")
        include_directories(${hd})

        # also add include path to my depencenies
        add_include_dependency(${hd})        
      endforeach()
    endif()
  endforeach()
endfunction(private_add_project_headers)

#
# This function generates makefile code for a standard library
#
# if the library is built as standard or static library is 
# determined by the cache parameter LIBRARY_BUILD_TYPE
#
# The ${PROJECT_NAME} is used to derive several variables.
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(add_standard_lib)
  private_add_standard_lib(${LIBRARY_BUILD_TYPE} ${ARGN})
endfunction(add_standard_lib)

#
# This function generates a makefile code for a standard library
#
# The ${PROJECT_NAME} is used to derive several variables.
# library_type specifies if a shared or a static library shall be built.
#
# CMake Parameters:
#  PKG - When the PKG parameter is given the tool will be added to the specified package
#
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(private_add_standard_lib library_type)
  cmake_parse_arguments(INSTALL "" "PKG" "" ${ARGN})
  # if specified INSTALL_PKG holds the tools installation package name
  # unrecognized parameters can be found in INSTALL_UNPARSED_ARGUMENTS

  set(TARGET_NAME "${PROJECT_NAMESPACE}${PROJECT_NAME}")

  # The source files
  file(GLOB ${PROJECT_NAME}_srcs "source/*.cpp" "source/*.C" "source/*.c")

  # Path for internal header files
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

  # generate version info and create dependency of lib
  add_version_info()

  assert_library_type(${library_type})
  add_library(${PROJECT_NAME} ${library_type}
    ${${PROJECT_NAME}_srcs}
  )

  # add link libraries from unparsed arguments
  private_add_project_libraries(${PROJECT_NAME})

  # Handle the test cases
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
  endif()

  # Handle the example code
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
  endif()

  # Installation instructions for the header files
  install_headers()
  install_library(${library_type})

  # set build output properties (only for dynamic libs)
  set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/lib"
    OUTPUT_NAME ${PROJECT_NAMESPACE}${PROJECT_NAME}
  )

endfunction(private_add_standard_lib)

#
# This function generates makefile code for a standard module
#
# if the library is built as standard or static library is 
# determined by the cache parameter LIBRARY_BUILD_TYPE
#
# The ${PROJECT_NAME} is used to derive several variables.
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(add_standard_module)
  private_add_standard_module(${LIBRARY_BUILD_TYPE} ${ARGN})
endfunction(add_standard_module)

#
# This function generates a makefile code for a standard module
#
# The ${PROJECT_NAME} is used to derive several variables.
# library_type specifies if a shared or a static library shall be built.
#
# CMake Parameters:
#  PKG - When the PKG parameter is given the tool will be added to the specified package
#
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(private_add_standard_module library_type)
  cmake_parse_arguments(INSTALL "" "PKG" "" ${ARGN})
  # if specified INSTALL_PKG holds the tools installation package name
  # unrecognized parameters can be found in INSTALL_UNPARSED_ARGUMENTS

  # The source files
  file(GLOB ${PROJECT_NAME}_srcs "source/*.cpp" "source/*.C" "source/*.c")

  # Path for internal header files
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

  # add version info dependency
  add_version_info()

  private_add_project_headers()

  # create library
  assert_library_type(${library_type})
  add_library(${PROJECT_NAME} ${library_type}
    ${${PROJECT_NAME}_srcs}
  )

  # add link libraries from unparsed arguments
  private_add_project_libraries(${PROJECT_NAME})

  # Handle the test cases
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
  endif()

  # Handle the example code
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
  endif()

  # Installation instructions for the header files and the library
  install_headers()
  install_library(${library_type})

  # set build output properties (only for dynamic modules)
  set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/modules"
    OUTPUT_NAME ${PROJECT_NAMESPACE}module_${PROJECT_NAME}
  )
endfunction(private_add_standard_module)

#
# This function generates makefile code for a standard tool
#
# The ${PROJECT_NAME} is used to derive several variables
#
# CMake Parameters:
#  PKG - When the PKG parameter is given the tool will be added to the specified package
#  TYPE - Specifies if the tool gets installed to bin or sbin. 
#         Accepted values: "sbin" for sbin, everything else results in bin
#
# Additional parameters ${ARGN} specify libraries, which get linked.
#
# e.g.: add_standard_tool(PKG core common ldb)
#       will put the tool ${PROJECT_NAME} into the core package
#       common & ldb will be used as link libraries
#
function(add_standard_executable)
  cmake_parse_arguments(INSTALL "" "PKG;TYPE" "" ${ARGN})
  # if specified INSTALL_PKG holds the tools installation package name
  # unrecognized parameters can be found in INSTALL_UNPARSED_ARGUMENTS

  # The source files
  file(GLOB ${PROJECT_NAME}_srcs "source/*.cpp" "source/*.C" "source/*.c")

  # Path for internal header files
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

  # add version info dependency
  add_version_info()

  private_add_project_headers()

  # The binary of the tool
  add_executable(${PROJECT_NAME} 
    ${${PROJECT_NAME}_srcs}
  )

  # add link libraries from unparsed arguments
  set(link_libraries "")

  # add link libraries from unparsed arguments
  private_add_project_libraries(${PROJECT_NAME})

  # Link libraries
  target_link_libraries(${PROJECT_NAME} 
    ${link_libraries}
  )

  # Installation instructions for the library
  if( "${INSTALL_TYPE}" STREQUAL "sbin")
    install_special_binary()
  else()
    install_binary()
  endif()

  # set build output properties (only for executables)
  set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/bin"
  )
endfunction(add_standard_executable)

#
# This function generates makefile code for a traditional third party library
#
# The ${PROJECT_NAME} is used to derive several variables
#
# CMake Parameters:
#  NAME - Name of the package in the download folder (mandatory)
#  VERSION - Version of the package in the download folder (mandatory)
#  PKG - When the PKG parameter is given the tool will be added to the specified package
#  TYPE - Specifies if the tool gets installed to bin or sbin. 
#         Accepted values: "sbin" for sbin, everything else results in bin
#
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(add_traditional_third_party)
  cmake_parse_arguments(TRD "" "PKG;TYPE;NAME;VERSION" "TARGETS;INSTALL_TARGETS;PATCHES" ${ARGN})
  # if specified TRD_PKG holds the tools installation package name
  # unrecognized parameters can be found in TRD_UNPARSED_ARGUMENTS

  set(PROJECT_NAMESPACE "" PARENT_SCOPE)

  if(NOT DEFINED TRD_NAME)
    log_fatal_error("NAME not set!")
  elseif(NOT DEFINED TRD_VERSION)
    log_fatal_error("VERSION not set!")
  endif()

  log_info("Adding traditional third party library: ${PROJECT_NAME} - ${TRD_NAME}")

  if(DEFINED TRD_PATCHES)
    log_debug("Applying Patch: ${TRD_PATCHES}")

    ExternalProject_Add(${PROJECT_NAME}
      URL ${CMAKE_CURRENT_SOURCE_DIR}/download/${TRD_NAME}
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ./configure --prefix=${TRD_STAGE_DIR}
      PATCH_COMMAND cat ${TRD_PATCHES} | patch --verbose && chmod +x configure
      BUILD_COMMAND make ${TRD_TARGETS}
      INSTALL_DIR ${TRD_STAGE_DIR}
      INSTALL_COMMAND make ${TRD_INSTALL_TARGETS}
    )
#  --strip=1
  else()
    ExternalProject_Add(${PROJECT_NAME}
      URL ${CMAKE_CURRENT_SOURCE_DIR}/download/${TRD_NAME}
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ./configure --prefix=${TRD_STAGE_DIR}
      BUILD_COMMAND make ${TRD_TARGETS}
      INSTALL_DIR ${TRD_STAGE_DIR}
      INSTALL_COMMAND make ${TRD_INSTALL_TARGETS}
    )
  endif()  
endfunction(add_traditional_third_party)

#
# This function generates makefile code for a traditional third party library
#
# The ${PROJECT_NAME} is used to derive several variables
#
# CMake Parameters:
#  NAME - Name of the package in the download folder (mandatory)
#  VERSION - Version of the package in the download folder (mandatory)
#  LIBS - Artifacts of the library (mandatory)
#  PKG - When the PKG parameter is given the tool will be added to the specified package
#  TYPE - Specifies if the tool gets installed to bin or sbin. 
#         Accepted values: "sbin" for sbin, everything else results in bin
#
# Additional parameters ${ARGN} specify libraries, which get linked.
#
function(add_cmake_third_party)
  cmake_parse_arguments(TRD "" "PKG;TYPE;NAME;VERSION;LIBS" "TARGETS;INSTALL_TARGETS;PATCHES;PARAMETERS" ${ARGN})
  # if specified TRD_PKG holds the tools installation package name
  # unrecognized parameters can be found in TRD_UNPARSED_ARGUMENTS

  set(PROJECT_NAMESPACE "" PARENT_SCOPE)

  if(NOT DEFINED TRD_NAME)
    log_fatal_error("NAME not set!")
  elseif(NOT DEFINED TRD_VERSION)
    log_fatal_error("VERSION not set!")
  elseif(NOT DEFINED TRD_LIBS)
    log_debug("LIBS not set!")
  endif()

  log_info("Adding cmake third party library: ${PROJECT_NAME} - ${TRD_NAME}")

  ExternalProject_Add( ${PROJECT_NAME}
    URL ${CMAKE_CURRENT_SOURCE_DIR}/download/${TRD_NAME}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${TRD_STAGE_DIR} ${TRD_PARAMETERS}
  )

  set(libraries "")
  if(TRD_LIBS)
    foreach(lib ${TRD_LIBS})
      list(APPEND libraries ${TRD_STAGE_DIR}/lib/${lib})
    endforeach()
  endif()
  set(${PROJECT_NAME}_LIBS ${libraries} CACHE INTERNAL "Libraries of ${PROJECT_NAME}")
endfunction(add_cmake_third_party)

function(add_glsl_shader)
  if(NOT TARGET ${PROJECT_NAME})
    log_debug("adding custom target ${PROJECT_NAME}")
    add_custom_target(${PROJECT_NAME})
  endif()

  file(GLOB SHADER_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  )
  file(GLOB SHADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.glsl
  )

  set(OUTPUT_HEADER ${CMAKE_BINARY_DIR}/output/shaders/${PROJECT_NAME}/include)
  set(OUTPUT_SHADER ${CMAKE_BINARY_DIR}/output/shaders/${PROJECT_NAME}/source)

  foreach(file_path ${SHADER_HEADER_FILES})
    get_filename_component(file "${file_path}" NAME)
    log_debug("adding copy command for ${file} to target ${PROJECT_NAME}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${file_path} ${OUTPUT_HEADER}/${file}
    )
  endforeach()

  foreach(file_path ${SHADER_FILES})
    get_filename_component(file "${file_path}" NAME)
    log_debug("adding copy command for ${file} to target ${PROJECT_NAME}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${file_path} ${OUTPUT_SHADER}/${file}
    )
  endforeach()

  private_add_project_headers()
endfunction(add_glsl_shader)

function(add_textures)

  set(OUTPUT_TEXTURES ${CMAKE_BINARY_DIR}/output/textures)
  
  add_custom_command(OUTPUT ${OUTPUT_TEXTURES}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/* ${OUTPUT_TEXTURES}
  )
  log_debug("adding custom target ${PROJECT_NAME}")
  add_custom_target(${PROJECT_NAME} ALL
    DEPENDS ${OUTPUT_TEXTURES}
  )

endfunction(add_textures)

function(use_shader)
  foreach(shader ${ARGN})
    log_debug("adding dependency to ${shader}")
    add_dependencies(${PROJECT_NAME} ${shader})
  endforeach()
endfunction(use_shader)

#
# This function just adds the given string to the CXX flag
#
# Remarks: don't use this function if you don't know what you are doing
#
function(add_c_flags flags)
  log_info("Adding '${flags}' to existing CMAKE_C_FLAGS: '${CMAKE_C_FLAGS}'")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flags}" PARENT_SCOPE)
endfunction(add_c_flags)

#
# This function just adds the given string to the CXX flag
#
# Remarks: don't use this function if you don't know what you are doing
#
function(add_cxx_flags flags)
  log_info("Adding '${flags}' to existing CMAKE_CXX_FLAGS: '${CMAKE_CXX_FLAGS}'")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags}" PARENT_SCOPE)
endfunction(add_cxx_flags)

#
# This function modifies the optimization level in the CXX and CC flags
# Supported levels: 0, 1, 2, 3, s, fast
#
# Remarks: Expects PROJECT_NAME to be set for logging
#
function(force_optimization level)
  if(${level} STREQUAL "s")
    set(checked_level ${level})
  elseif(${level} STREQUAL "fast")
    set(checked_level ${level})
  elseif(${level} GREATER -1 AND ${level} LESS 4)
    set(checked_level ${level})
  else()
    log_fatal_error("Used wrong optimization level: ${level} on ${PROJECT_NAME}")
  endif()

  log_info("Forcing optimization level -O${checked_level}")

  string(REGEX REPLACE "\\-O|\\-O.|\\-Ofast" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "\\-O|\\-O.|\\-Ofast" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O${checked_level}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O${checked_level}" PARENT_SCOPE)
endfunction(force_optimization)

#
# This function forces a specific standard in the compilers C flags
# given by the argument "standard_name"
#
# Remarks: Expects PROJECT_NAME to be set for logging
#
function(force_c_std standard_name)
  log_info("Forcing C standard -std=${standard_name}")

  string(REGEX REPLACE "\\-std=[a-zA-Z0-9+:]+" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=${standard_name}" PARENT_SCOPE)
endfunction(force_c_std)

#
# This function forces a specific standard in the compilers CXX flags
# given by the argument "standard_name"
#
# Remarks: Expects PROJECT_NAME to be set for logging
#
function(force_cxx_std standard_name)
  log_info("Forcing CXX standard -std=${standard_name}")

  string(REGEX REPLACE "\\-std=[a-zA-Z0-9+:]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${standard_name}" PARENT_SCOPE)
endfunction(force_cxx_std)