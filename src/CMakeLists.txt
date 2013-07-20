# Written in 2009 by Lloyd Hilaiel
#
# License
#
# All the cruft you find here is public domain.  You don't have to credit
# anyone to use this code, but my personal request is that you mention
# Igor Pavlov for his hard, high quality work.
#

IF (WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF (WIN32)

FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h *.hpp)
FILE(GLOB PUB_HDRS *.h *.hpp)
# set up some paths for outputing the usable binaries
SET (libDir
     ${CMAKE_CURRENT_BINARY_DIR}/../${LIBDUKASCOPY_DIST_NAME}/lib)
SET (incDir
     ${CMAKE_CURRENT_BINARY_DIR}/../${LIBDUKASCOPY_DIST_NAME}/include/${NINETY47})
SET (testDir
     ${CMAKE_CURRENT_BINARY_DIR}/../${LIBDUKASCOPY_DIST_NAME}/test)

# an include directory to allow implementation to find public headers
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# get the built libs into the correct place
SET(LIBRARY_OUTPUT_PATH ${libDir})

ADD_LIBRARY(dukascopy_s STATIC ${SRCS} ${HDRS})
ADD_LIBRARY(dukascopy   SHARED ${SRCS} ${HDRS})

# setup shared library version numbering
SET_TARGET_PROPERTIES(
    dukascopy PROPERTIES
    SOVERSION ${LIBDUKASCOPY_MAJOR}
    VERSION ${LIBDUKASCOPY_MAJOR}.${LIBDUKASCOPY_MINOR}.${LIBDUKASCOPY_MICRO})

# on win32 we'll need to setup exports correctly
SET(sharedLibCompileFlags "-DLIBDUKASCOPY_SHARED -DLIBDUKASCOPY_BUILD")
IF (APPLE)
  SET(sharedLibCompileFlags "${sharedLibCompileFlags} -fno-common")
ENDIF (APPLE)

SET_TARGET_PROPERTIES(dukascopy PROPERTIES
                      COMPILE_FLAGS ${sharedLibCompileFlags})

# create these output directories
FILE(MAKE_DIRECTORY ${libDir})
FILE(MAKE_DIRECTORY ${incDir})
FILE(MAKE_DIRECTORY ${testDir})

# copy public headers to output directory
FOREACH (header ${PUB_HDRS})
  # preserve relative pathing
  ADD_CUSTOM_COMMAND(TARGET dukascopy_s POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${header} ${incDir})
ENDFOREACH (header ${PUB_HDRS})
