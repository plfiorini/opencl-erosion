include(logging)

find_file(
  version_gen_hpp_in
  "version_gen.hpp.in"
  PATH "${CMAKE_HOME_DIRECTORY}/cmake/templates"
  DOC "input template file for generation of version_gen.hpp"
)

if(NOT version_gen_hpp_in)
  message(FATAL "Cannot find version_gen.hpp.in")
endif()

find_file(
  version_gen_fwd_hpp_in
  "version_gen_fwd.hpp.in"
  PATH "${CMAKE_HOME_DIRECTORY}/cmake/templates"
  DOC "input template file for generation of version_gen_fwd.hpp"
)

if(NOT version_gen_fwd_hpp_in)
  message(FATAL "Cannot find version_gen_fwd.hpp.in")
endif()

find_file(
  version_gen_cpp_in
  "version_gen.cpp.in"
  PATH "${CMAKE_HOME_DIRECTORY}/cmake/templates"
  DOC "input template file for generation of version_gen.cpp"
)

if(NOT version_gen_cpp_in)
  message(FATAL "Cannot find version_gen.cpp.in")
endif()

find_program(date_tool "date"         DOC "the common date tool")

execute_process(COMMAND ${date_tool} "+%Y.%m.%d" OUTPUT_VARIABLE version_info_DATE  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${date_tool} "+%Y"       OUTPUT_VARIABLE version_info_YEAR  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${date_tool} "+%-m"      OUTPUT_VARIABLE version_info_MONTH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${date_tool} "+%-d"      OUTPUT_VARIABLE version_info_DAY   OUTPUT_STRIP_TRAILING_WHITESPACE)

find_package(Git)

if(NOT GIT_FOUND)
  message(FATAL_ERROR "git not found")
endif()

#
# Set variable <target> to <value> if defined and not an empty string, else to <default>
#   target ... variable, which gets assigned
#   value  ... the user value
#   default... default value
#
function(version_set_value_or_default target value default)
  if(DEFINED value)
    if (value STREQUAL "")
        set(${target} "${default}" PARENT_SCOPE)
    else()
        set(${target} "${value}" PARENT_SCOPE)
    endif()
  else()
    set(${target} "${default}" PARENT_SCOPE)
  endif()
endfunction()

#
# Get the output of the tool 'svnversion'
#   status  ...   <rev-number><status-modifier>
#   number  ...   <rev-number>
#
function(get_git_version version)
  set(output "")
  execute_process(COMMAND ${GIT_EXECUTABLE} "rev-list" "HEAD" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE output OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(${version} "${output}" PARENT_SCOPE)
endfunction()

#
# Function for version info generation 
#
function(generate_version_info)

  # get svn revision via svnversion tool (outputs e.g. <rev-number>M for modified source tree)
  get_git_version(git_version)

  if( NOT EXISTS "${generate_version_gen_hpp}" OR
      NOT EXISTS "${generate_version_gen_fwd_hpp}" OR
      NOT EXISTS "${generate_version_gen_cpp}" )
    set(VERSION "" CACHE INTERNAL "Version String" FORCE)
  endif()

  if(VERSION)
    if ("${VERSION}" STREQUAL "${git_version}")
      log_info("Version information has not changed - skipping file generation")
      return()
    endif()
  endif()

  log_info("Version information has changed ('${VERSION}' -> '${git_version}') - rebuilding version info files ...")
  set(VERSION "${git_version}" CACHE INTERNAL "Version String" FORCE)

  set(top_level_generation_path "include")
  set(GENERATION_DIR generated/${top_level_generation_path})

  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${GENERATION_DIR})

  set(generation_path ${CMAKE_CURRENT_BINARY_DIR}/${GENERATION_DIR})

  set(generate_version_gen_hpp     ${generation_path}/version_gen.hpp)
  set(generate_version_gen_fwd_hpp ${generation_path}/version_gen_fwd.hpp)
  set(generate_version_gen_cpp     ${CMAKE_CURRENT_BINARY_DIR}/version_gen.cpp)

  version_set_value_or_default(PROJECT_NAME       "${version_info_PROJECT_NAME}"       "${PROJECT_NAME}")
  version_set_value_or_default(PROJECT_DATE       "${version_info_PROJECT_DATE}"       "${version_info_DATE}")
  version_set_value_or_default(PROJECT_COMMENT    "${version_info_PROJECT_COMMENT}"    "<undefined>")           #FIXME

  version_set_value_or_default(BUILD_DATE         "${version_info_BUILD_DATE}"         "${version_info_DATE}")
  version_set_value_or_default(BUILD_TYPE         "${version_info_BUILD_TYPE}"         "${CMAKE_BUILD_TYPE}")
  version_set_value_or_default(BUILD_COMMENT      "${version_info_BUILD_COMMENT}"      "<undefined>")                #FIXME   

  # set revision number 
  set(VERSION "${git_version}")

  # set compiler version
  set(C_COMPILER_VERSION    "${C_COMPILER_VERSION}")
  set(CXX_COMPILER_VERSION  "${CXX_COMPILER_VERSION}")

  # set platform name
  set(PLATFORM_VERSION "${CMAKE_SYSTEM_NAME}, ${CMAKE_SYSTEM_PROCESSOR}")

  # Generate version info files
  configure_file(${version_gen_hpp_in}     ${generate_version_gen_hpp})
  configure_file(${version_gen_fwd_hpp_in} ${generate_version_gen_fwd_hpp})
  configure_file(${version_gen_cpp_in}     ${generate_version_gen_cpp})

  # set version info implementation in parent scope
  set(generate_version_gen_hpp ${generate_version_gen_hpp} PARENT_SCOPE)
  set(generate_version_gen_fwd_hpp ${generate_version_gen_fwd_hpp} PARENT_SCOPE)
  set(generate_version_gen_cpp ${generate_version_gen_cpp} PARENT_SCOPE)

endfunction()

function(add_version_info)

  # Establish dependency by adding the generated files to the list of source files

  set(project_srcs "${PROJECT_NAME}_srcs")
  if(NOT DEFINED ${project_srcs})
    message(STATUS "generate_version_info: Cannot establish dependency since ${project_srcs} is not defined")
  endif()

  list(APPEND ${project_srcs} ${generate_version_gen_cpp})
  list(APPEND ${project_srcs} ${generate_version_gen_hpp})
  list(APPEND ${project_srcs} ${generate_version_gen_fwd_hpp})

  # Exporting the value of ${project_srcs} to parent scope
  set(${project_srcs} ${${project_srcs}} PARENT_SCOPE)

  # Add output path to the include directories
  include_directories(${CMAKE_CURRENT_BINARY_DIR})

endfunction()
