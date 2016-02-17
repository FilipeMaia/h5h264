#!/usr/bin/env python

import h5py
import numpy as np

# plist = h5py.h5p.create(h5py.h5p.FILE_ACCESS)
# plist.set_cache(0, 10007, 2*8*1024*1024*10, 0.5)
# fid = h5py.h5f.open('comp.h5', fapl=plist)
# f = h5py.File(fid)
f = h5py.File('comp.h5','r')
print "%d" % (np.sum(f['h264']))
f.close()
