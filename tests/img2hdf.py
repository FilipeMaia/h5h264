#!/usr/bin/env python

import h5py
import sys
from scipy.misc import imread

if len(sys.argv) < 3:
    print "Usage: img2hdf.py <output> <img1> ... <imgN>"
    sys.exit(1)

a = imread(sys.argv[2])
width = a.shape[1]
height = a.shape[0]
item_size = None
if(a.dtype == 'uint8'):
    item_size = 1
item_size = 2

plist = h5py.h5p.create(h5py.h5p.FILE_ACCESS)
nimgs = len(sys.argv)-2
chunk_frames = min(20,nimgs)
plist.set_cache(0, 10007, chunk_frames*item_size*width*height*2, 0.5)
fid = h5py.h5f.create(sys.argv[1], fapl=plist)
f = h5py.File(fid)
#dset = f.create_dataset("data", (nimgs, height, width), compression=32010, dtype=a.dtype,
#                        chunks=(chunk_frames, height, width), compression_opts=(height,width,item_size))
dset = f.create_dataset("data", (nimgs, height, width), compression=32010, dtype='uint16',
                        chunks=(chunk_frames, height, width), compression_opts=(height,width,item_size))

for (i,fname) in enumerate(sys.argv[2:]):
    a = imread(fname)
    print "Reading %s" %(fname)
    dset[i,:,:] = a[:,:,0]+a[:,:,1]*256
f.close()

fid2 = h5py.h5f.create('zip.h5', fapl=plist)
f2 = h5py.File(fid2)
dset2 = f2.create_dataset("data", (nimgs, height, width), compression='gzip', dtype='uint16',
                         chunks=(chunk_frames, height, width))


for (i,fname) in enumerate(sys.argv[2:]):
    a = imread(fname)
    print "Reading %s" %(fname)
    dset2[i,:,:] = a[:,:,0]+a[:,:,1]*256
f2.close()

