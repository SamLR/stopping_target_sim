#!/usr/bin/env python
# encoding: utf-8
"""
beam_analysis.py

Created by Sam Cook on 2012-03-28.
Copyright (c) 2012 . All rights reserved.
"""

from ROOT import TH1F
from time import sleep
from sys import exit

def quickTH1(name, bins=None, xmin=0, xmax=400):
    if not bins: bins=xmax - xmin
    return TH1F(name, name, bins, xmin, xmax)
    
def momCalc(x, y, z):
    return (x**2 + y**2 + z**2)**0.5

def main():
	file_name = "../input/particle_dist_100M_charged.txt"
	
	data_fmt = ("event", "pid", "x", "y", "z", "px", "py", "pz", "t")
	momentum = quickTH1("muon momentum at end of beam pipe")
	muoncount = 0
	at_centre = 0
	centre_mom = quickTH1("muon momentum in stopping target area")
	
	with open(file_name) as in_file:
	    for line in in_file:
	        data = dict(zip(data_fmt, [float(i) for i in line.split()] ))
	        if not (abs(data['pid']) == 13): continue #muons only
	        momentum.Fill(momCalc(data['px'], data['py'], data['pz']))
	        muoncount += 1
	        if (-18.5 < data['x'] < 18.5) or (-4 < data ['y'] < 4): 
	            at_centre += 1
    	        centre_mom.Fill(momCalc(data['px'], data['py'], data['pz']))
	
	print muoncount
	momentum.Draw()
	sleep(60)
	        
	        


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt, k:
        exit(1)

