import h5py
import numpy as np

f = h5py.File('simple.h5','w')
width = 1024
height = 1024
nframes = 10
data = np.zeros((nframes, height, width), dtype=np.int32)
# The compression keyword argument tells h5py that we want to use the h5h264 filter
dset = f.create_dataset("data", (nframes, height, width), compression=32010, data=data,
                        chunks=(5, height, width), compression_opts=(height, width, data.dtype.itemsize))
f.close()
