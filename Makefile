all: h264_dec_enc h5h264.o test

cflags = -g

H264_Decoder.o: H264_Decoder.cpp
	c++ $(cflags) -c H264_Decoder.cpp -I/Users/filipe/src/ffmpeg/build/include

h264_dec_enc.o: h264_dec_enc.cpp
	c++ $(cflags) -c h264_dec_enc.cpp -I/Users/filipe/src/ffmpeg/build/include

h264_dec_enc: h264_dec_enc.o H264_Decoder.o
	c++ -o h264_dec_enc h264_dec_enc.o H264_Decoder.o -lavcodec -lavutil -L/Users/filipe/src/ffmpeg/build/lib 

h5h264.o: h5h264.c encode.c decode.c
	cc $(cflags) -c h5h264.c

test: test.o 
	cc $(cflags) -o test test.o -lavcodec -lavutil

test.o: test.c encode.c decode.c
	cc $(cflags) -c test.c

clean:
	rm *.o
