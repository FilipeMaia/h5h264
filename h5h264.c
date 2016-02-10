#include <stdlib.h>
#include <H5PLextern.h>

#include "decode.c"
#include "encode.c"

#define H5Z_FILTER_H264 32010

static size_t H5Z_filter_h264(unsigned int flags, size_t cd_nelmts,
			      const unsigned int cd_values[], size_t nbytes,
			      size_t *buf_size, void **buf);


const H5Z_class2_t H5Z_H264[1] = {
  {
    /* H5Z_class_t version */
    H5Z_CLASS_T_VERS,
    /* Filter id number */
    (H5Z_filter_t)H5Z_FILTER_H264, 
    /* encoder_present flag (set to true) */
    1, 
    /* decoder_present flag (set to true) */
    1, 
    /* Filter name for debugging    */
    "HDF5 H264 filter; see http://www.hdfgroup.org/services/contributions.html", 
    /* The "can apply" callback     */
    NULL, 
    /* The "set local" callback     */
    NULL, 
    /* The actual filter function   */
    (H5Z_func_t)H5Z_filter_h264, 
  }
};

H5PL_type_t H5PLget_plugin_type(void){
  return H5PL_TYPE_FILTER;
}

const void *H5PLget_plugin_info(void) {
  return H5Z_H264;
}


/* flags defines the direction of the filter
   cd_nelmts is the number of user defined values
   cd_values are the user defined values
   nbytes is the size of the input
   buf_size must contain the size of the output when returning
   *buf points to the input data at the start and should be set
   to point to the output data before returning. 
   N.B. The input data should be freed before returning.
*/
static size_t H5Z_filter_h264(unsigned int flags, size_t cd_nelmts,
			      const unsigned int cd_values[], size_t nbytes,
			      size_t *buf_size, void **buf){
    fprintf(stderr,"Calling h5h264\n");
  //  fprintf(stderr,"cd_nelmts %zu\n", cd_nelmts);
  //  fprintf(stderr,"flags %d\n", flags);

  char *output_buffer = NULL;

  if (flags & H5Z_FLAG_REVERSE) {
    /* Decompress data */
    output_buffer = h264_decode(*buf, nbytes, buf_size);
  }else{
    /* Compress data */
    if(cd_nelmts != 3){
      /* We're missing the cd values */
      fprintf(stderr, "cd_values are missing\n");
      return 0;
    }
    int height = cd_values[0];
    int width = cd_values[1];
    int item_size = cd_values[2];
    if(nbytes % (height*width*item_size)){
      /* The cd values must be wrong */
      fprintf(stderr, "cd_values do not match input size\n");
      fprintf(stderr, "height - %d width - %d item_size - %d, nbytes - %zu\n", height, width, item_size, nbytes);
      return 0;
    }
    output_buffer = h264_encode(*buf, nbytes, height, width,
				item_size, buf_size);
    //    printf("DEBUG: encoded %zu bytes into %zu bytes\n", nbytes, *buf_size);
  }
  if(!output_buffer){
    return 0;
  }
  free(*buf);
  *buf = output_buffer;
  return *buf_size;
}
