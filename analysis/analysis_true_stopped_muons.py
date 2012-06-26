#!/usr/bin/env python
# encoding: utf-8
"""
analysis_true_stopped_muons.py

Created by Sam Cook on 2012-04-02.
Copyright (c) 2012 . All rights reserved.
"""

#! /usr/bin/python2.7  

# As much as anything this is a quick framework for me to figger out the best 
# way of running the analysis. Depending on proformance I may re write this in
# C++/C to be compiled. 

from ROOT import TFile, TTree, TH1F, TCanvas
from collections import OrderedDict
from time import sleep
from sys import exit

def cut(branch, values):
    return True if branch in values else False

def getDataEvent(tree, data, index):
    if (tree.GetEntry(index) <= 0): return # no data, shouldn't happen
    # if not cut(tree.pid, (-13, -11, 11, 13)): return
    key = tree.eventNo
    value = {'PID'   :tree.pid,
             'parent':tree.ParentID,
             'track' :tree.trackID,
             'pos'   :(tree.posX, tree.posY, tree.posZ),
             'mom'   :(tree.momX, tree.momY, tree.momZ),
             'time'  :tree.time,
             'place' :tree.GetName()}
    if key in data:
        data[key].append(value)
    else:
        data[key] = [value,]
    return

def calcMom(x,y,z):
    return (x*x+y*y+z*z)**0.5

def quickTH1(name, bins=None, xmin=0, xmax=250):
    if not bins: bins=xmax-xmin
    return TH1F(name, name, bins, xmin, xmax)

def main():
    file_in = TFile('../output/truth_out.root', 'READ')
    treeA = file_in.Get("monA")
    treeB = file_in.Get("monB")

    nentriesA = treeA.GetEntries()
    nentriesB = treeB.GetEntries()

    data={}

    # loop over all entries
    for index in range(nentriesA if nentriesA>nentriesB else nentriesB):
        if index < nentriesA: 
            # save the information from monitorA
            getDataEvent(treeA, data, index)
            
        if index < nentriesB: 
            # also save the information from monitor B
            getDataEvent(treeB, data, index)
        
    # create an ordered dictionary of events
    eventOrderedDict = OrderedDict(data)
    decay_count = 0
    muon_count = 0
    momDistStopped = quickTH1("stopped momentum distribution")
    stoppedTimes = quickTH1("decay times", xmin=0, xmax=2000, bins=2000)
    # allTimes = quickTH1("times", xmin=-100, xmax=20000, bins=100)
    # lowTimes = quickTH1("times", xmin=-100, xmax=200, bins=100)
    # highTimes = quickTH1("times", xmin=200, xmax=20000, bins=100)
    # firstTimes = quickTH1("firsttimes", xmin=-100, xmax=3000000000, bins=200)
    times = []
    zero_count = 0
    for value in eventOrderedDict.values(): #don't really care about the event number
        event = sorted(value, key=lambda s: s['time']) #alias and time order
        # if len(event) < 2: continue # not enough particles seen to ID decaying muons
        muon_tracks = {} # dict of muon track IDs matched to momentums & times
        decayed = []
        # first_time = event[0]['time']
        # continue
        for hit in event:
            # # step through in time, look for muons in monB and then electrons with muon parents
            # delta = hit['time'] - first_time
            # if delta == 0: continue
            # allTimes.Fill(delta)
            # if delta < 200: 
            #     lowTimes.Fill(delta)
            # else:
            #     highTimes.Fill(delta)
            # 
            # continue
            if abs(hit['PID']) == 13 and hit['parent'] not in muon_tracks:
            # if abs(hit['PID']) == 13 and hit['track'] not in muon_tracks:
                # is it a new muon?
                muon_count += 1
                muon_tracks[hit['track']] = [calcMom(*hit['mom']), hit['time']]
            elif (abs(hit['PID']) == 11) and (hit['parent'] in muon_tracks) and (hit['parent'] not in decayed):
                # look for electrons, who have a muon parent and have not already decayed
                # decay electron
                delta = hit['time'] - muon_tracks[hit['parent']][1]

                decay_count += 1
                decayed.append(hit['parent'])
                momDistStopped.Fill(muon_tracks[hit['parent']][0])
                if delta == 0: 
                    zero_count += 1
                    continue
                stoppedTimes.Fill(delta)
                
        # if decayed: print event
        
    print "decays count =", decay_count, "zero count =", zero_count
    print "total events", len(eventOrderedDict)
    # print min(times), max(times), float(sum(times))/len(times)
    # firstTimes.Draw()
    # allTimes.Draw()
    momDistStopped.Draw()
    canvas = TCanvas("c2", "c2")
    canvas.cd()
    # lowTimes.Draw()
    # canvas = TCanvas("c3", "c3")
    # canvas.cd()
    # highTimes.Draw()
    stoppedTimes.Draw()

    sleep(600)        

    
    
if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt, k:
        exit(1)    
    
    
    
