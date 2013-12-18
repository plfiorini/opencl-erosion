
# set installer prefix
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE INTERNAL "Set the install prefix")

# library (& module) build type
set(LIBRARY_BUILD_TYPE  "SHARED"  CACHE INTERNAL  "Set the library build type (STATIC or SHARED)")

set(PROJECT_NAMESPACE   "mkay_"    CACHE INTERNAL  "Prefix for project artifacts")

set(PROJECT_DEPENDENCY_POSTFIX "_dependant_headers" CACHE INTERNAL "Postfix for project header dependencies")
