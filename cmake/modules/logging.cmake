#
# Wrapper function for generating fatal error output
# The function just calls message() with appropriate parameters
# to create a common look and feel for the output.
# All additional parameters ${ARGN} are passed to message()
# without modification.
#
# If the function is called outside a project, "generic" is used
# as the project name.
#
# message() itself will stop execution of the cmake run.
#
function(log_fatal_error)
  if(NOT PROJECT_NAME)
    set(project_name "generic")
  else()
    set(project_name "${PROJECT_NAME}")
  endif()

  message(FATAL_ERROR "${project_name}: ${ARGN}")
endfunction(log_fatal_error)

#
# Wrapper function for generating warning output
# The function just calls message() with appropriate parameters
# to create a common look and feel for the output.
# All additional parameters ${ARGN} are passed to message()
# without modification.
#
# If the function is called outside a project, "generic" is used
# as the project name.
#
function(log_warning)
  if(NOT PROJECT_NAME)
    set(project_name "generic")
  else()
    set(project_name "${PROJECT_NAME}")
  endif()

  message(WARNING "${project_name}: ${ARGN}")
endfunction(log_warning)

#
# Wrapper function for generating informational output
# The function just calls message() with appropriate parameters
# to create a common look and feel for the output.
# All additional parameters ${ARGN} are passed to message()
# without modification.
#
# If the function is called outside a project, "generic" is used
# as the project name.
#
function(log_info)
  if(NOT PROJECT_NAME)
    set(project_name "generic")
  else()
    set(project_name "${PROJECT_NAME}")
  endif()

  message(STATUS "INFO: ${project_name}: ${ARGN}")
endfunction(log_info)

#
# Wrapper function for generating debug output
# The function just calls message() with appropriate parameters
# to create a common look and feel for the output.
# The output is only generated if ENABLE_LOG_DEBUG is true or
# if the environment variable ENABLE_LOG_DEBUG is defined.
#
# All additional parameters ${ARGN} are passed to message()
# without modification.
# If the function is called outside a project, "generic" is used
# as the project name.
#
function(log_debug)
  set(DO_PRINT "no")

  if($ENV{ENABLE_LOG_DEBUG})
    set(DO_PRINT "yes")
  endif()

  if(${ENABLE_LOG_DEBUG})
    set(DO_PRINT "yes")
  endif()

  if(${DO_PRINT})
    if(NOT PROJECT_NAME)
      set(project_name "generic")
    else()
      set(project_name "${PROJECT_NAME}")
    endif()

    message(STATUS "DEBUG: ${project_name}: ${ARGN}")
  endif()
endfunction(log_debug)

#
# Wrapper function for generating fixme output
# The function just calls message() with appropriate parameters
# to create a common look and feel for the output.
# The output is only generated if ENABLE_LOG_FIXME is true.
#
# All additional parameters ${ARGN} are passed to message()
# without modification.
# If the function is called outside a project, "generic" is used
# as the project name.
#
function(log_fixme)
  if(${ENABLE_LOG_FIXME})
    if(NOT PROJECT_NAME)
      set(project_name "generic")
    else()
      set(project_name "${PROJECT_NAME}")
    endif()

    message(STATUS "FIXME: ${project_name}: ${ARGN}")
  endif()
endfunction(log_fixme)

