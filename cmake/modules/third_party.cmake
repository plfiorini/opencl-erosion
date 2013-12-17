#
# This function extends the include path for the use of the sqlite3 library
# for the project defined by ${PROJECT_NAMESPACE}${PROJECT_NAME}
# Also the library name is appended to the variable ${}_dependant_libs
# for the use in dependent applications
#
function(use_sqlite3)
  find_path(SQLITE3_INCLUDE_PATH sqlite3.h
    PATHS /usr/include/
    DOC "Include path for header files of sqlite3"
  )

  find_library(SQLITE3_LIB libsqlite3.so
    PATHS /usr/lib
    DOC "Fully qualified library name of the sqlite3 library"
  )

  log_debug("SQLITE3_INCLUDE_PATH=${SQLITE3_INCLUDE_PATH}")
  log_debug("SQLITE3_LIB=${SQLITE3_LIB}")

  if(NOT EXISTS ${SQLITE3_INCLUDE_PATH})
    log_fatal_error("sqlite3 header files not found")
  endif()

  if(NOT EXISTS ${SQLITE3_LIB})
    log_fatal_error("libsqlite3.so library not found")
  endif()

  include_directories(
    SYSTEM ${SQLITE3_INCLUDE_PATH}
  )

  target_link_libraries(${PROJECT_NAME}
    ${SQLITE3_LIB}
  )
endfunction(use_sqlite3)

function(use_system_library name)
  log_info("use_system_library: Searching for ${name}")

  find_library(${name}_LIB ${name}
    PATHS /usr/lib /usr/lib64
    DOC "Fully qualified library path of ${name}"
  )

  if(NOT EXISTS ${${name}_LIB})
    log_fatal_error("use_system_library: ${name} library not found")
  else()
    log_info("use_system_library: Using library ${${name}_LIB}")
  endif()

  target_link_libraries(${PROJECT_NAME}
    ${${name}_LIB}
  )
endfunction(use_system_library)

function(use_third_party_cmake name)
  log_info("use_third_party_cmake: Searching libraries of ${name}")
  foreach(lib ${${name}_LIBS})
    if(NOT EXISTS ${lib})
      log_fatal_error("${lib} library not found")
    else()
      log_info("use_third_party_cmake: Using library ${lib}")
    endif()

    target_link_libraries(${PROJECT_NAME}
      ${lib}
    )
  endforeach()
endfunction(use_third_party_cmake)