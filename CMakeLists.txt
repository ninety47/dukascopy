cmake_minimum_required(VERSION 2.8.7)

project(libdukascopy)
set(NINETY47 ninety47)
set(LIBDUKASCOPY_MAJOR 0)
set(LIBDUKASCOPY_MINOR 1)
set(LIBDUKASCOPY_MICRO 0)
set(LIBDISTNAME 
    "${NINETY47}-dukascopy-${LIBDUKASCOPY_MAJOR}.${LIBDUKASCOPY_MINOR}.${LIBDUKASCOPY_MICRO}")
set(LIBDIR "${CMAKE_CURRENT_BINARY_DIR}/${LIBDISTNAME}/lib")
set(INCDIR "${CMAKE_CURRENT_BINARY_DIR}/${LIBDISTNAME}/include/${NINETY47}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LIBDISTNAME}/lib)
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/${LIBDISTNAME}/lib")

# External library dependencies...
# Set these paths if you have boost installed in a non-standard location, for example:
# set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/opt/mse/modules/boost/1.56.0/include")
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/opt/mse/modules/boost/1.56.0/lib")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USER_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem unit_test_framework)

# Build dependencies
add_subdirectory(lib/easylzma)

# Shared object and static library building
file(GLOB lib_src "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/src/*.cpp")
include_directories("${CMAKE_SOURCE_DIR}/include" ${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib/easylzma/src)
add_library(dukascopy SHARED ${lib_src})
add_library(dukascopy_s STATIC ${lib_src})

set_target_properties(
    dukascopy PROPERTIES
    OUTPUT_NAME "${NINETY47}_dukascopy"
    SOVERSION ${LIBDUKASCOPY_MAJOR}
    VERSION ${LIBDUKASCOPY_MAJOR}.${LIBDUKASCOPY_MINOR}.${LIBDUKASCOPY_MICRO}
)
set_target_properties(dukascopy_s PROPERTIES
    OUTPUT_NAME "${NINETY47}_dukascopy"
)

target_link_libraries(dukascopy ${Boost_LIBRARIES} easylzma_s)
target_link_libraries(dukascopy_s ${Boost_LIBRARIES} easylzma_s)
# Copy over public header...
file(MAKE_DIRECTORY ${INCDIR})
file(COPY ./include/ninety47/dukascopy.h DESTINATION ${INCDIR})

#
# dukascopy utils..
#

#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LIBDISTNAME}/bin)
#add_executable(hello ./bin/main.cpp)
#set_target_properties(hello  PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
#    COMPILE_FLAGS "-I./include"
#)
#target_link_libraries(hello world)


#
# Tests....
#
file(GLOB tests_src RELATIVE "${CMAKE_SOURCE_DIR}/test" "${CMAKE_SOURCE_DIR}/test/*.cpp")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test)
file(COPY ./test/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    foreach(tp ${tests_src})
        string(REGEX REPLACE ".cpp" "" target ${tp})
        add_executable(${target} "./test/${tp}")
        set_target_properties(
            ${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test"
            COMPILE_FLAGS "-I./include"
        )
        target_link_libraries(${target} dukascopy ${Boost_LIBRARIES} easylzma)
    endforeach(tp)
endif()

