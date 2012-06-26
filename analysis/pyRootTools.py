#!/usr/bin/env python
# encoding: utf-8
"""
pyRootTools.py

Created by Sam Cook on 2012-04-02.
Copyright (c) 2012 . All rights reserved.
"""

import sys
import os
import unittest

def getDataEvent(tree, data, index):
    if (tree.GetEntry(index) <= 0): return # no data, shouldn't happen
    key = tree.eventNo
    value = {'PID'   :tree.pid,
             'parent':tree.ParentID,
             'track' :tree.trackID,
             'pos'   :(tree.posX, tree.posY, tree.posZ),
             'mom'   :(tree.momX, tree.momY, tree.momZ),
             'time'  :tree.time,
             'place' :tree.GetName()}
    if data.has_key(key): # if the event exists append to it otherwise create it
        data[key].append(value)
    else:
        data[key] = [value,]
    return


def calcMom(x,y,z):
    return (x*x+y*y+z*z)**0.5

def quickTH1(name, bins=250, xmin=0, xmax=250):
    return TH1F(name, name, bins, xmin, xmax)

class pyRootTools:
	def __init__(self):
		pass


class pyRootToolsTests(unittest.TestCase):
	def setUp(self):
		pass


if __name__ == '__main__':
	unittest.main()