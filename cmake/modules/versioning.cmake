include(logging)

find_program(date_tool "date"         DOC "the common date tool")
find_program(svnversion_tool "svnversion" DOC "the git working copy version tool")

execute_process(COMMAND ${date_tool} "+%Y.%m.%d" OUTPUT_VARIABLE version_info_DATE  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${date_tool} "+%Y"       OUTPUT_VARIABLE version_info_YEAR  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${date_tool} "+%-m"      OUTPUT_VARIABLE version_info_MONTH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${date_tool} "+%-d"      OUTPUT_VARIABLE version_info_DAY   OUTPUT_STRIP_TRAILING_WHITESPACE)

find_package(Git)

if(NOT Subversion_FOUND)
  message(FATAL_ERROR "git tool not found")
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
function(get_svn_revision revision)
  set(output "")
  execute_process(COMMAND ${svnversion_tool} "${PROJECT_SOURCE_DIR}" OUTPUT_VARIABLE output OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(${revision} "${output}" PARENT_SCOPE)
endfunction()

#
# Function for version info generation 
#
function(generate_version_info)

  # get svn revision via svnversion tool (outputs e.g. <rev-number>M for modified source tree)
  get_svn_revision(svn_revision)

  if( NOT EXISTS "${generate_version_gen_hpp}" OR
      NOT EXISTS "${generate_version_gen_fwd_hpp}" OR
      NOT EXISTS "${generate_version_gen_cpp}" )
    set(SVN_REVISION "" CACHE INTERNAL "IDS SVN Revision String" FORCE)
  endif()

  if(SVN_REVISION)
    if ("${SVN_REVISION}" STREQUAL "${svn_revision}")
      log_info("Version information has not changed - skipping file generation")
      return()
    endif()
  endif()

  log_info("Version information has changed ('${SVN_REVISION}' -> '${svn_revision}') - rebuilding version info files ...")
  set(SVN_REVISION "${svn_revision}" CACHE INTERNAL "IDS SVN Revision String" FORCE)

  # get full git info about current project
  Subversion_WC_INFO(${PROJECT_SOURCE_DIR} module)

  # Extract directory name from the project name 
  # (by removing  prefix and adding ids/ later)
  #string(REGEX REPLACE "" "" top_level_generation_path ${PROJECT_NAME})
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

  if("${BUILD_SVN_REVISION}" STREQUAL "0")
    set(BUILD_COMMENT "${BUILD_COMMENT}, sources not checked in")
  endif()

  # set revision number 
  set(VERSION "${svn_revision}")

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
