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
    

def main():
    file_in = TFile('../output/mppc_out.root', 'READ')
    treeA1 = file_in.Get("mppcA1")
    treeA2 = file_in.Get("mppcA2")
    treeB1 = file_in.Get("mppcB1")
    treeB2 = file_in.Get("mppcB2")
    coincidenceDt = 100 # time window to be considered coincident
    tdcDt = 2000 # time window for subsequent hits (once B hit without A)
    
    trees = (treeA1, treeA2, treeB1, treeB2)
    entries = tuple([i.GetEntries() for i in trees])

    dataEvent={}
    # Create the event dictionary
    for index in range(max(entries)):
        for tree in trees:
            if index < entries[trees.index(tree)]: 
                # boiler plate checks that the data exists
                getDataEvent(tree, dataEvent, index)

    # data should now have all the muons and electrons from A&B
    eventOD = OrderedDict(dataEvent)
    photonNDist = quickTH1("Distribution of photons detected")
    for event in eventOD.values(): photonNDist.Fill(len(event))
    
    possible_muons = 0
    # create a list of hits
    hits = [(hit['time'], hit['place']) for event in dataEvent.values() for hit in event]
    hits = sorted(hits, key=lambda x: x[0]) #time sort
    print len(hits)
    # for i in hits: print i
    start_time = 0 # the current tdc window start time
    dts = quickTH1("time differences", 200, 0, 2000)
    triggers = resets = adds = 0
    last_hits = {'A1':None, 'A2':None, 'B1':None, 'B2':None} # record the time of the last hit for each side
    for hit in hits:
        dt = hit[0] - start_time
        if dt > tdcDt:
            # look for B && !A
            b_hits = a_hits = []
            if hit[1][:5] == 'mppcB': 
                triggers += 1
                start_time = hit[0]
        elif dt < coincidenceDt:
            # check that we only have hits in B if in A reset start_time
            if hit[1][:5] == 'mppcA': 
                resets += 1
                start_time = 0
            else: # must be another hit in mppcB
                b_hits.append(hit[0]) # at least one hit in b since trigger
        elif (dt < tdcDt):
            # add the hit to the tdc?
            # create the string for the mppc on the opposite side of the scintillator 
            opposite_mppc = hit[1][4] + ('1' if hit[1][5]=='2' else '2')
            this_dt = hit[0] - last_hits[opposite_mppc]
            if this_dt > coincidenceDt:
                # this hit is not coincident with the previous hits on the opposite mppc
                side_to_side_dt = abs(last_hits[hit[1][4]+'1'] - last_hits[hit[1][4]+'2'])
                if side_to_side_dt < coincidenceDt:
                    # the previous pair of hits on this scint were coincidence; record it!
                    dt = (min(last_hits[hit[1][4]+'1'], last_hits[hit[1][4]+'2'])) - start_time
                    dts.Fill(dt)
                    adds +=1
            last_hits[hit[1][4:]] = hit[0]
            # adds += 1
            # dts.Fill(dt)
    
    print 'triggers', triggers
    print 'resets', resets
    print 'adds', adds
    dts.Draw()
    sleep(60)
            
    
    
    
if __name__=='__main__':
    try:
        main()
    except KeyboardInterrupt, k:
        exit(1)
    
    
    
    