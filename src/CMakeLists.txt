add_library(h5h264 SHARED h5h264.c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(h5h264 ${FFMPEG_LIBRARIES})