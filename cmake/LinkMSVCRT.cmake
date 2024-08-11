function(find_newest_msvcrt OUTVAR)
  file(TO_CMAKE_PATH $ENV{WINDIR} WINDIR)
  file(GLOB INSTALLED_MSVCRT_RUNTIMES "${WINDIR}/System32/msvcr*.dll")
  set(INSTALLED_MSVCRT_NAMES)

  foreach(MSVCRT ${INSTALLED_MSVCRT_RUNTIMES})
    get_filename_component(MSVCRT ${MSVCRT} NAME_WE)
    list(APPEND INSTALLED_MSVCRT_NAMES ${MSVCRT})
  endforeach()

  foreach(VERSION 110 100 90) # try newest first
    list(FIND INSTALLED_MSVCRT_NAMES "msvcr${VERSION}" INDEX)
    if(INDEX GREATER -1)
      set(${OUTVAR} ${VERSION} PARENT_SCOPE)
      return()
    endif()
  endforeach()

  message(FATAL_ERROR "No compatible MSVCRT runtime installed")
endfunction()