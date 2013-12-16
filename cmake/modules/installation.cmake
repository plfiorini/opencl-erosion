
# set installer prefix
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE INTERNAL "Set the install prefix" FORCE)

#
# set the INSTALL_PKG var if unset
#
function(set_pkg_if_unset default)
    # if not defined, set to default
    if( NOT DEFINED INSTALL_PKG )
        set(INSTALL_PKG "${default}" PARENT_SCOPE)
    # if empty set to default
    elseif( "${INSTALL_PKG}" STREQUAL "" )
        set(INSTALL_PKG "${default}" PARENT_SCOPE)
    endif()

endfunction(set_pkg_if_unset)

#
# install IDS header files, but exclude external files
#
function(install_headers)
    set_pkg_if_unset("dev")
    if (ENABLE_INSTALL_INTERNAL_HEADERS)
        install(DIRECTORY include/ids 
          DESTINATION output/include 
          COMPONENT ${INSTALL_PKG}
          PATTERN ".svn" EXCLUDE)
    else()
        if(EXTERNAL_VISIBLE)
            install(DIRECTORY include/ids 
              DESTINATION include 
              COMPONENT ${INSTALL_PKG}
              REGEX "/.svn/|_impl.hpp" EXCLUDE)
        endif()
    endif()
endfunction(install_headers)

#
# install special binary
#
# package specifies the installer package
#
function(install_special_binary)
    set_pkg_if_unset("tools")
    install(TARGETS ${PROJECT_NAME} 
      DESTINATION sbin
      COMPONENT ${INSTALL_PKG})
endfunction(install_special_binary)

#
# install binary
#
# package specifies the installer package
#
function(install_binary)
    set_pkg_if_unset("tools")
    install(TARGETS ${PROJECT_NAME} 
      DESTINATION bin
      COMPONENT ${INSTALL_PKG})
endfunction(install_binary)

#
# install library
#
function(install_library library_type)
    if ( "${library_type}" STREQUAL "STATIC" )
        set(INSTALL_PKG "dev")
    else()
        set(INSTALL_PKG "core")
    endif()
    install(TARGETS ${PROJECT_NAME} 
      DESTINATION lib
      COMPONENT ${INSTALL_PKG})
endfunction(install_library)


