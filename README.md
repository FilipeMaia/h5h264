Compression filter for HDF5 files using H.264 compressions.
It uses H.264 lossless compression through `ffmpeg`.

It performs well on sequences of related images.

When used to compress Flash X-ray Imaging diffraction data from LCLS it compresses the data to about 1/3 of the original size compared to 2/3 of the size for gzip compression, with similar performance.

To built it use `cmake`.
It depends on `ffmpeg` with h.264 encoding support.

To be able to use the filter you need to set `HDF5_PLUGIN_PATH` to point to the location of the newly built `libh5h264.so`/`libh5h264.dylib`.

For more information about HDF5 filters check this [PDF](https://www.hdfgroup.org/HDF5/doc/Advanced/DynamicallyLoadedFilters/HDF5DynamicallyLoadedFilters.pdf)
