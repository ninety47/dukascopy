
FILE(GLOB SRCS test_*.cpp)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}/../${LIBDUKASCOPY_DIST_NAME}/include
)
LINK_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}/../${LIBDUKASCOPY_DIST_NAME}/lib
)
SET(unitTestFlags "-DDEBUG")


FOREACH (unit_test ${SRCS})
  STRING(REGEX REPLACE ".cpp" "" unit_test_target ${unit_test})
  ADD_EXECUTABLE(${unit_test_target} ${unit_test})
  TARGET_LINK_LIBRARIES(${unit_test_target} dukascopy_s)
  GET_TARGET_PROPERTY(binPath ${unit_test_target} LOCATION)
#  ADD_CUSTOM_TARGET(test ${binPath})
ENDFOREACH (unit_test ${SRCS})


