#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>

static char * h264_decode(void * data, size_t size, size_t * output_size){
  /* The beginning of the data has the output size*/
  memcpy(output_size, data, sizeof(size_t));
  data += sizeof(size_t);
  size -= sizeof(size_t);

  char * output = malloc(*output_size);
  char * output_ptr = output;
  avcodec_register_all();
  AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_H264);
  if(!codec) {
    fprintf(stderr, "Cannot find the h264 codec\n");
    return NULL;
  }
  AVCodecContext* codec_context = avcodec_alloc_context3(codec);
  if (!codec_context) {
    fprintf(stderr, "Could not allocate video codec context\n");
    return NULL;
  }
  if(codec->capabilities & CODEC_CAP_TRUNCATED) {
    codec_context->flags |= CODEC_FLAG_TRUNCATED;
  }

  if(avcodec_open2(codec_context, codec, NULL) < 0) {
    fprintf(stderr, "Could not open codec.\n");
    return NULL;
  }

  AVCodecParserContext* parser = av_parser_init(AV_CODEC_ID_H264);
  if(!parser) {
    fprintf(stderr,"Cannot create H264 parser.\n");
    return NULL;
  }

  AVFrame * frame = av_frame_alloc();
  if (!frame) {
    fprintf(stderr, "Could not allocate frame\n");
    return NULL;
  }  

  uint8_t* frame_data = NULL;
  int frame_size = 0;
  int frame_count = 0;
  int last_frame = 0;
  while(1){
    int parse_len = av_parser_parse2(parser, codec_context, &frame_data, &frame_size, 
				   data, size, 0, 0, AV_NOPTS_VALUE);
    AVPacket pkt;
    int got_picture = 0;
    int decode_len = 0;
    
    av_init_packet(&pkt);
    
    pkt.data = frame_data;
    pkt.size = frame_size;
    
    decode_len = avcodec_decode_video2(codec_context, frame, &got_picture, &pkt);
    if(decode_len < 0) {
      fprintf(stderr, "Error while decoding a frame.\n");
    }
    
    if(got_picture == 0) {
      if(!last_frame){
	/* The first failure is probably due to last frame
	   Let's give it another try */
	last_frame = 1;
	continue;
      }else{
	break;
      }
    }
    data += frame_size;
    size -= frame_size;
    for (int y = 0; y < frame->height; y++) { 
      memcpy(output_ptr, &frame->data[0][y * frame->linesize[0]], frame->width);
      output_ptr += frame->width;
    } 
  }

  av_parser_close(parser);
  avcodec_close(codec_context);
  av_free(codec_context);
  av_free(frame);
  return output;
}
