#!/usr/bin/env python

import h5py
import numpy as np

# plist = h5py.h5p.create(h5py.h5p.FILE_ACCESS)
# plist.set_cache(0, 10007, 2*4*1024*1024*10, 0.5)
# fid = h5py.h5f.create('comp.h5', fapl=plist)
# f = h5py.File(fid)
f = h5py.File('comp.h5','w')

data = np.zeros((10,1024,1024))
dset = f.create_dataset("h264", (10, 1024, 1024), compression=32010, data=data,
                        chunks=(2, 1024, 1024), compression_opts=(1024,1024,4))
buf = np.zeros((1,1024,1024))
for i in range(0,10):
    buf = dset[i,:,:]
f.close()
