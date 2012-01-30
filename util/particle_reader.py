#!/usr/bin/python
"""Reads in a 6D particle distribution file and spits it out in an human friendly way"""

filename = "../input/music_particle_dist.txt"

sumx = 0
sumy = 0
sumz = 0
count = 0

with open(filename) as infile:
    for line in infile:
        line = [float(i) for i in line.split(' ')]
        sumx += line[2]
        sumy += line[3]
        sumz += line[4]
        count +=1
        # print "%6i| %4.0f | %4.0f | %4.0f | %4.1f | %4.1f | %4.1f"%tuple(line)
        
print "aveX = %.2f;\t aveY = %.2f;\t aveZ = %.2f, count=%i"%(float(sumx)/count, float(sumy)/count, float(sumz)/count, count)