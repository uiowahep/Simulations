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
    image = [[[0 for i in range(30)] for j in range(30)] for k in range(30)]
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
#    inp = RT.TFile("/Users/vk/software/simulations/data/hgcal/root/20161125_2351/hgcaldata__60__1448372482.root")
    pathToFile = "/Users/vk/software/simulations/data/hgcal/root/20161125_2351"
    #fileName = "hgcaldata__8__352344210.root"
#    fileName = "hgcaldata__60__1448372482.root"
#    fileName = "hgcaldata__50__1243131744.root"
    fileName = "hgcaldata__4__601838791.root"
    energy = 4
    inp = RT.TFile(os.path.join(pathToFile, fileName))
    tree = inp.Get("Events")
    data = []
    n = tree.GetEntries()
    for i in range(n):
        tree.GetEntry(i)
        data.append(partitionImage(tree.emresponseImage))

    a = np.array(data)
    np.save("test", a)
    print a[0]
    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    xs,ys,zs,rs = departitionImage(a[0])
    ax.scatter(xs,ys,zs, c='r', marker='o')
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Layer")
    ax.set_title("CALICE-like SiW %dGeV e- Hits" % energy)

    plt.show()

if __name__=="__main__":
    main()
