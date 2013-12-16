#
# This function extends the include path for the use of the sqlite3 library
# for the project defined by ${PROJECT_NAMESPACE}${PROJECT_NAME}
# Also the library name is appended to the variable ${}_dependant_libs
# for the use in dependent applications
#
function(use_sqlite3)

  set(TARGET_NAME "${PROJECT_NAMESPACE}${PROJECT_NAME}")

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

  target_link_libraries(${TARGET_NAME} 
    ${SQLITE3_LIB}
  )

  list(APPEND ${PROJECT_NAME}${LIBRARY_DEPENDENCY_POSTFIX} "${SQLITE3_LIB}")

endfunction()
