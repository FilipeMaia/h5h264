cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW) # CMake 3.0
endif()


project (h5h264)
set(CMAKE_BUILD_TYPE Release)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(FFMPEG)
include_directories(${FFMPEG_INCLUDE_DIR})


ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)
