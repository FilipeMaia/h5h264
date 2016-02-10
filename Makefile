all: h264_dec_enc h5h264.o test libh5h264.so.0.1

CFLAGS = -g -fPIC
MAJOR = 0
MINOR = 1
NAME1 = h5h264
VERSION = $(MAJOR).$(MINOR)

lib: lib$(NAME1).so.$(VERSION)

$(NAME1).o: $(NAME1).c
	$(CC) $(CFLAGS) $(INCLUDES) -c $(NAME1).c

lib$(NAME1).so.$(VERSION): $(NAME1).o
	$(CC) -shared -Wl,-soname,lib$(NAME1).so.$(MAJOR) $^ -o $@ -lavcodec -lavutil

H264_Decoder.o: H264_Decoder.cpp
	c++ $(CFLAGS) -c H264_Decoder.cpp -I/Users/filipe/src/ffmpeg/build/include

h264_dec_enc.o: h264_dec_enc.cpp
	c++ $(CFLAGS) -c h264_dec_enc.cpp -I/Users/filipe/src/ffmpeg/build/include

h264_dec_enc: h264_dec_enc.o H264_Decoder.o
	c++ -o h264_dec_enc h264_dec_enc.o H264_Decoder.o -lavcodec -lavutil -L/Users/filipe/src/ffmpeg/build/lib 

#h5h264.o: h5h264.c encode.c decode.c
#	cc $(CFLAGS) -c h5h264.c

test: test.o 
	cc $(CFLAGS) -o test test.o -lavcodec -lavutil

test.o: test.c encode.c decode.c
	cc $(CFLAGS) -c test.c

clean:
	rm *.o *.so*
