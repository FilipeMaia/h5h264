ADD_EXECUTABLE(test_enc_dec test_enc_dec.c)
include_directories(${CMAKE_SOURCE_DIR}/src)
TARGET_LINK_LIBRARIES(test_enc_dec ${FFMPEG_LIBRARIES})