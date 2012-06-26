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
    if not cut(tree.pid, (-13, -11, 11, 13)): return
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

def quickTH1(name, bins=None, xmin=0, xmax=250):
    if not bins: bins=xmax-xmin
    return TH1F(name, name, bins, xmin, xmax)
    

def main():
    file_in = TFile('../output/truth_out.root', 'READ')
    treeA = file_in.Get("monA")
    treeB = file_in.Get("monB")

    nentriesA = treeA.GetEntries()
    nentriesB = treeB.GetEntries()

    dataEvent={}

    # loop over all entries
    for index in range(nentriesA if nentriesA>nentriesB else nentriesB):
        # boiler plate checks that the data exists
        if index < nentriesA: 
            # save the information from monitorA
            getDataEvent(treeA, dataEvent, index)
            
        if index < nentriesB: 
            # also save the information from monitor B
            getDataEvent(treeB, dataEvent, index)
        
    # data should now have all the muons and electrons from A&B
    eventOD = OrderedDict(dataEvent)
    muon_count = 0
    # momDistB = quickTH1("negative muons at A")
    momDistB = quickTH1("Muon momentums at the near scintillator", bins=125)
    # momDistA = quickTH1("positive muons at A")
    momDistA = quickTH1("muons (-) at A")
    momDistStopped = quickTH1("stopped momentum distribution")
    stoppedTimes = quickTH1("decay times", xmin=2, xmax=2000)

    for eventID in eventOD:
        event = eventOD[eventID] # alias
        # if len(event) < 2:
        #     # Don't just skip empty events, still could towards number of muons
        #     if (abs(event[0]['PID']) == 13): muon_count+=1 
        #     continue
        
        event = sorted(event, key=lambda s: s['time']) # time sort the event

        muon_tracks = [] # list of 'used' muons to prevent double counting etc
        stopped = []
        parentID = -1
        already_seen_B =[]
        ignore = [] 
        for hit in event:
            Simple macro to plot momentum distribution of muons at B
            if abs(hit['PID']) != 13: continue
            if hit['place'] == 'monB' and hit['track'] not in already_seen_B:
                momDistB.Fill(calcMom(*hit['mom']))
                muon_count += 1
                already_seen_B.append(hit['track'])
            
            # if not (abs(hit['PID']) == 13 or abs(hit['PID']) == 11):continue # quick filter for electrons and muons
            # if (abs(hit['PID']) == 13) and (hit['place'] == 'monB'): # prospective muon for stopping
            #     if hit['track'] not in already_seen_B:
            #         stopped.append(hit)
            #         already_seen_B.append(hit['track'])
            #         # print hit['track']
            # elif (hit['PID'] == 11): 
            #     for muon in stopped:
            #         if muon['track'] == hit['parent'] and hit['parent'] not in ignore:
            #             momDistStopped.Fill(calcMom(*muon['mom']))
            #             stoppedTimes.Fill(hit['time']-muon['time'])
            #             print (hit['time']-muon['time']), hit['parent']
            #             ignore.append(hit['parent'])
            #             print 'muon', stopped
            #             print 'electron', hit
            #             print '*'*40
        # print '*'*40
        
    print muon_count
    print "total events", len(eventOD)
    momDistB.Draw()

    sleep(600)        

    
    
if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt, k:
        exit(1)    
    
    
    
    