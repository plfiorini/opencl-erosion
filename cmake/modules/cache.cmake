
# set installer prefix
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE INTERNAL "Set the install prefix")

set(WINDOWS_LIBRARY_BASE_PATH "D:\\development" CACHE PATH  "Base path for third party libraries on windows")

# library (& module) build type
if(WIN32)
  set(LIBRARY_BUILD_TYPE  "STATIC"  CACHE INTERNAL  "Set the library build type (STATIC or SHARED)")
else()
  set(LIBRARY_BUILD_TYPE  "SHARED"  CACHE INTERNAL  "Set the library build type (STATIC or SHARED)")
endif()

set(TRD_STAGE_DIR "${CMAKE_BINARY_DIR}/stage/third_party" CACHE INTERNAL "Stage Directory for third party libs")

set(PROJECT_NAMESPACE   "mkay_"    CACHE INTERNAL  "Prefix for project artifacts")

set(PROJECT_HEADER_DEPENDENCY_POSTFIX "_dependant_headers" CACHE INTERNAL "Postfix for project header dependencies")
set(PROJECT_DLL_DEPENDENCY_POSTFIX "_dependant_dlls" CACHE INTERNAL "Postfix for project dll dependencies")

add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DSDL_MAIN_HANDLED)

# set & create output directories
set(STATICLIB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(SHAREDLIB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(MODULE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/modules)

if(NOT EXISTS ${STATICLIB_OUTPUT_DIRECTORY})
  file(MAKE_DIRECTORY ${STATICLIB_OUTPUT_DIRECTORY})
endif()
if(NOT EXISTS ${SHAREDLIB_OUTPUT_DIRECTORY})
  file(MAKE_DIRECTORY ${SHAREDLIB_OUTPUT_DIRECTORY})
endif()
if(NOT EXISTS ${MODULE_OUTPUT_DIRECTORY})
  file(MAKE_DIRECTORY ${MODULE_OUTPUT_DIRECTORY})
endif()