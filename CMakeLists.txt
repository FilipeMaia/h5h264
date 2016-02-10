cmake_minimum_required(VERSION 2.8)

project (h5h264)
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(h5h264 SHARED h5h264.c)
target_link_libraries(h5h264 -lavcodec -lavutil)