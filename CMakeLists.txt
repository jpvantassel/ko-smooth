project(smooth LANGUAGES CXX)

# Require C++14-compliant compiler; only available for CMake v3.10 and up
set(CMAKE_CXX_STANDARD 14)

cmake_minimum_required(VERSION 3.10)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

# General compile settings
IF (NOT CMAKE_BUILD_TYPE)
 SET(CMAKE_BUILD_TYPE "debug")
 #SET(CMAKE_BUILD_TYPE "release")
ENDIF (NOT CMAKE_BUILD_TYPE)

# GNU specific settings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
endif()

# Intel specific settings
if (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

# Clang specific settings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-var-template")
endif()

# pthreads
find_package (Threads)
link_libraries (${CMAKE_THREAD_LIBS_INIT})

# Include directories
include_directories(BEFORE
  ${smooth_SOURCE_DIR}/include/
)

# Include source
SET(smooth_src
  ${smooth_SOURCE_DIR}/src/csvtools.cpp
  ${smooth_SOURCE_DIR}/src/numtools.cpp
  ${smooth_SOURCE_DIR}/src/smooth.cpp
)

if (SMOOTH_BUILD_LIB)
  add_library(lsmooth SHARED ${smooth_src})
  add_executable(smooth ${smooth_SOURCE_DIR}/src/main.cpp)
  target_link_libraries(smooth lsmooth)
else()
  add_executable(smooth ${smooth_SOURCE_DIR}/src/main.cpp ${smooth_src})
endif()