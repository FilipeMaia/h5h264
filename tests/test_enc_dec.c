#include <libavutil/time.h>
#include "decode.c"
#include "encode.c"


uint8_t * gen_data(int width, int height, int nframes){
  uint8_t * data = malloc(width*height*nframes);
  for (int i = 0; i < nframes; i++){
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
	data[i * width * height + y * width + x] = x + y + i * 3;
      }
    }
  }
  return data;
}

int main(){
  int width = 1000;
  int height = 1000;
  int nframes = 200;
  int size = width*height*nframes;
  char * data = (char *)gen_data(width, height, nframes);
  size_t encoded_size;
  int data_sum = 0;
  for(int i = 0;i<size;i++){
    data_sum += data[i];
  }
  int64_t tic = av_gettime();
  char * encoded = h264_encode(data, size, height, width,
			       1, &encoded_size);
  int64_t toc = av_gettime();
  float dt = (float)(toc-tic)/1.0e6;
  printf("Encoded %d bytes in %f s - %g bytes/s\n", size, dt, size/dt);

  size_t decoded_size;
  tic = av_gettime();
  char * decoded = h264_decode(encoded, encoded_size, &decoded_size);
  toc = av_gettime();
  dt = (float)(toc-tic)/1.0e6;
  printf("Decoded %d bytes in %f s - %g bytes/s\n", size, dt, size/dt);
  if(memcmp(data, decoded, size)){
    for(int i = 0;i<size;i++){
      if(data[i] != decoded[i]){
	printf("Bytes %d differ! %d and %d\n", i, data[i], decoded[i]);
      }
    }
  }
  return 0;
}
