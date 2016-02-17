Compression filter for HDF5 files using H.264 compression.
It uses H.264 lossless compression through [ffmpeg](http://ffmpeg.org).

It performs well on sequences of related images.

When used to compress Flash X-ray Imaging diffraction data from LCLS it compresses the data to about 1/3 of the original size compared to 2/3 of the size for gzip compression, with similar performance.

# Installation

Clone the repository and build using `cmake`.
It depends on an available [ffmpeg](http://ffmpeg.org) with H.264 encoding support.

```bash
git clone https://github.com:FilipeMaia/h5h264.git
cd h5h264
mkdir build
cd build
cmake ..
make
```

# Usage
The filter identification number is 32010. This value must be passed as the
`filter` argument to `H5Pset_filter`.

To be able to use the filter you also need to set `HDF5_PLUGIN_PATH` to point to the
location of the newly built `libh5h264.so`/`libh5h264.dylib`, e.g.:

```bash
export HDF5_PLUGIN_PATH=${HOME}/h5h264/build/src
```

if you had cloned the repository in your home directory.

For more information about HDF5 filters check this [PDF](https://www.hdfgroup.org/HDF5/doc/Advanced/DynamicallyLoadedFilters/HDF5DynamicallyLoadedFilters.pdf).


This filters requires 3 user options (the `cd_values` argument of
 `H5Pset_filter`). The height and width of the images to be
compressed and the size in bytes of each pixel of the image. It currently only
supports 1,2 or 4 byte pixels.

The tests directories contains examples on how to use this filter with h5py.

Here is a simple example:

```python
import h5py
import numpy as np

f = h5py.File('simple.h5','w')
width = 1024
height = 1024
nframes = 10
data = np.zeros((nframes, height, width), dtype=np.int32)
# The compression keyword argument tells h5py that we want to use the h5h264 filter.
dset = f.create_dataset("data", (nframes, height, width), compression=32010, data=data,
                        chunks=(5, height, width), compression_opts=(height, width, data.dtype.itemsize))
f.close()
```

Reading a compressed dataset requires no extra code;
