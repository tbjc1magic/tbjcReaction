import numpy as np


elist = []
xlist = []

with open("line1.dat") as f:
    content =  f.readlines()
    #print content
    for line in content:
        x,e = line.split()
        #print x,e
        elist = elist +[float(e)]
        xlist = xlist +[float(x)*100]

#print elist
#print xlist
print np.polyfit(xlist,elist,1)

