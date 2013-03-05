# - Try to find Cairomm
# Once done this will define
#
#  CAIROMM_FOUND - system has Cairomm
#  CAIROMM_INCLUDE_DIRS - the Cairomm include directory
#  CAIROMM_LIBRARIES - Link these to use BoehmGC
#  CAIROMM_DEFINITIONS - Compiler switches required for using BoehmGC
#
#  Copyright (c) 2013 Fabian Schuiki
#

if (CAIROMM_LIBRARIES AND CAIROMM_INCLUDE_DIRS)
  # in cache already
  set(CAIROMM_FOUND TRUE)
else (CAIROMM_LIBRARIES AND CAIROMM_INCLUDE_DIRS)
  find_path(CAIROMM_INCLUDE_DIR
    NAMES
      "cairomm/cairomm.h"
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    PATH_SUFFIXES
      "cairomm-1.0"
  )
  message(STATUS "cairomm.h: ${CAIROMM_INCLUDE_DIR}")

  find_library(CAIROMM_LIBRARY
    NAMES
      "cairomm-1.0.0"
      "cairomm-1.0"
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  if (CAIROMM_LIBRARY)
    set(CAIROMM_FOUND TRUE)
  endif (CAIROMM_LIBRARY)

  set(CAIROMM_INCLUDE_DIRS
    ${CAIROMM_INCLUDE_DIR}
  )

  if (CAIROMM_FOUND)
    set(CAIROMM_LIBRARIES
      ${CAIROMM_LIBRARIES}
      ${CAIROMM_LIBRARY}
    )
  endif (CAIROMM_FOUND)

  if (CAIROMM_INCLUDE_DIRS AND CAIROMM_LIBRARIES)
     set(CAIROMM_FOUND TRUE)
  endif (CAIROMM_INCLUDE_DIRS AND CAIROMM_LIBRARIES)

  if (CAIROMM_FOUND)
    if (NOT Cairomm_FIND_QUIETLY)
      message(STATUS "Found Cairomm: ${CAIROMM_LIBRARIES}")
    endif (NOT Cairomm_FIND_QUIETLY)
  else (CAIROMM_FOUND)
    if (Cairomm_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Cairomm")
    endif (Cairomm_FIND_REQUIRED)
  endif (CAIROMM_FOUND)

  # show the CAIROMM_INCLUDE_DIRS and CAIROMM_LIBRARIES variables only in the advanced view
  mark_as_advanced(CAIROMM_INCLUDE_DIRS CAIROMM_LIBRARIES)

endif (CAIROMM_LIBRARIES AND CAIROMM_INCLUDE_DIRS)

