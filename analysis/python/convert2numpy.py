#!/usr/bin/python

import ROOT as RT
import numpy as np
import os

# matplotlib
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

def partitionImage(arr):
    """ create a 3D tensor in simple lists"""
    iglobal = 0
    image = np.zeros((30,30,30))
    for ilayer in range(30):
        for iy in range(30):
            for ix in range(30):
                image[ilayer][iy][ix] = arr[iglobal]
                iglobal+=1
    return image

def departitionImage(image):
    """ returns (xs, ys, zs, rs) """
    xs = []; ys = []; zs = []; rs = []
    for ilayer in range(30):
        for iy in range(30):
            for ix  in range(30):
                if image[ilayer][iy][ix]>0:
                    xs.append(ix)
                    ys.append(iy)
                    zs.append(ilayer)
                    rs.append(image[ilayer][iy][ix])
    return (xs,ys,zs,rs)

def main():
    # chaining in
    print "chaining in.\n"
    chain = RT.TChain("Events")
    import glob
    files = glob.glob("/Users/vk/software/simulations/data/hgcal/root/20161126_1156/*.root")
    for f in files:
        chain.Add(f)
    n = chain.GetEntries()
    pixels = np.empty((0, 27001), dtype=np.double)
    data = []
    energies = [0 for x in range(61)]
    print "done chaining.\n"
    for i in range(n):
        if i%1000==0: print "Event %d / %d" % (i, n)
        chain.GetEntry(i)
        # assign energy
        energy = chain.energy

        # assign the 3d image as just list of pixels and energy as the 0element of the row
        # for now save only up to 500 events for each energy
        if energies[int(energy)]<500:
            resp = chain.emresponseImage
            arr = [x for x in resp]
            data.append([energy] + arr)
            energies[int(energy)]+=1

    npdata = np.array(data)
    np.save("/Users/vk/software/simulations/data/hgcal/npy/20161126_1156/test", npdata)

if __name__=="__main__":
    main()
