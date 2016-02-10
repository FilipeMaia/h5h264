#!/usr/bin/env python

import h5py

plist = h5py.h5p.create(h5py.h5p.FILE_ACCESS)
plist.set_cache(0, 10007, 2*4*1024*1024*10, 0.5)
fid = h5py.h5f.create('comp.h5', fapl=plist)
f = h5py.File(fid)
dset = f.create_dataset("h264", (10, 1024, 1024), compression=32010, 
                        chunks=(2, 1024, 1024), compression_opts=(1024,1024,4))
dset[:,:,:] = 1
f.close()
