
# set installer prefix
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE INTERNAL "Set the install prefix")

# library (& module) build type
set(LIBRARY_BUILD_TYPE  "SHARED"  CACHE INTERNAL  "Set the library build type (STATIC or SHARED)")

# prefix for project artifacts
set(PROJECT_NAMESPACE   "mkay_"    CACHE INTERNAL  "The project namespace")

# postfix for dependant libraries
set(LIBRARY_DEPENDENCY_POSTFIX   "_dependant_libs"    CACHE INTERNAL  "Postfix for library dependencies")