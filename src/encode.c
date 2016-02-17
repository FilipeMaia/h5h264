#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>

static char * h264_encode(char * data, size_t size, size_t height, size_t width,
			  int item_size, size_t * output_size){
  av_log_set_level(AV_LOG_WARNING);
  avcodec_register_all();
  int got_output;
  uint8_t endcode[] = { 0, 0, 1, 0xb7 };

  AVCodec * codec = avcodec_find_encoder(AV_CODEC_ID_H264);
  if (!codec) {
    fprintf(stderr, "H264 codec not found\n");
    return NULL;
  }
  AVCodecContext * c = avcodec_alloc_context3(codec);
  if (!c) {
    fprintf(stderr, "Could not allocate video codec context\n");
    return NULL;
  }

  if(item_size == 1){
    /* Nothing to do */
  }else if(item_size == 2){
    width *= 2;
  }else if(item_size == 4){
    width *= 2;
    height *= 2;
  }else{
    fprintf(stderr, "Item size must be 1, 2 or 4 bytes\n");
    return NULL;
  }

  /* resolution must be a multiple of two */
  if((height % 2) || (width % 2)){
    fprintf(stderr, "Height and width must be a multiple of two\n");
    return NULL;    
  }
  /* put sample parameters */
  c->width = width;
  c->height = height;
  c->pix_fmt = AV_PIX_FMT_YUV420P;

  /* Set the medium preset and qp 0 for lossless encoding */
  AVDictionary * param = 0;
  av_dict_set(&param, "qp", "0", 0);
  av_dict_set(&param, "preset", "medium", 0);

  /* open codec */
  if (avcodec_open2(c, codec, &param) < 0) {
    fprintf(stderr, "Could not open codec\n");
    return NULL;
  }

  AVFrame *frame = av_frame_alloc();
  if (!frame) {
    fprintf(stderr, "Could not allocate video frame\n");
    return NULL;
  }
  frame->format = c->pix_fmt;
  frame->width  = c->width;
  frame->height = c->height;
  
  int ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
		       c->pix_fmt, 32);
    /* Zero Cb and Cr */
  memset(frame->data[1], 0, c->height*frame->linesize[1]/2);
  memset(frame->data[2], 0, c->height*frame->linesize[2]/2);

  if (ret < 0) {
    fprintf(stderr, "Could not allocate raw picture buffer\n");
    return NULL;
  }   

  int nframes = size/(width*height);


  AVPacket * pkt = malloc(sizeof(AVPacket)*(nframes+1));

  int pkt_idx = 0;
  for (int i = 0; i < nframes+1; i++) {
    av_init_packet(&pkt[i]);
    // packet data will be allocated by the encoder
    pkt[i].data = NULL;
    pkt[i].size = 0;
  }

  for (int i = 0; i < nframes; i++) {
    /* Y */
    size_t idx = 0;
    for (int y = 0; y < height; y++) { 
      memcpy(&frame->data[0][y * frame->linesize[0]],
      	     data, width);
      data += width;
    } 
    /* Skip Cb and Cr */
    frame->pts = i;
    if(avcodec_encode_video2(c, &pkt[pkt_idx], frame, &got_output) < 0){
      fprintf(stderr, "Error encoding frame\n");
      return NULL;
    }

    if (got_output) {
      /* printf("Write frame %3d (size=%5d)\n", pkt_idx, pkt[pkt_idx].size); */
      pkt_idx++;
    }
  }

  /* get the delayed frames */
  for (got_output = 1; got_output; pkt_idx++) {
    ret = avcodec_encode_video2(c, &pkt[pkt_idx], NULL, &got_output);
    if (ret < 0) {
      fprintf(stderr, "Error encoding frame\n");
      return NULL;
    }    
  }

  /* Calculate output size*/
  /* We'll save the input size in the first 8 bytes */
  *output_size = sizeof(size);
  for (int i = 0; i < nframes; i++) {
    *output_size += pkt[i].size;
  }

  /* Allocate output */
  char * output = malloc(*output_size);
  char * ptr = output;
  memcpy(ptr, &size, sizeof(size));
  ptr += sizeof(size);
  /* Copy data to output and free packets */
  for (int i = 0; i < nframes; i++) {
    memcpy(ptr, pkt[i].data, pkt[i].size);
    ptr += pkt[i].size;
    av_packet_unref(&pkt[i]);
  }
  free(pkt);
  avcodec_close(c);
  av_free(c);
  av_freep(&frame->data[0]);
  av_frame_free(&frame);
  
  return output;
}
