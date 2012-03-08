#! /usr/bin/python2.7  

# As much as anything this is a quick framework for me to figger out the best 
# way of running the analysis. Depending on proformance I may re write this in
# C++/C to be compiled. 

from ROOT import TFile, TTree
from collections import OrderedDict

def cut(branch, values):
    return True if branch in values else False

def getDataFlat(tree, data, index):
    if (tree.GetEntry(index) <= 0): return # no data, shouldn't happen
    if not cut(tree.pid, (-13, -11, 11, 13)): return
    data.append({'event':tree.eventNo,
                 'PID'  :tree.pid,
                 'pos'  :(tree.posX, tree.posY, tree.posZ), # 3-vector
                 'time' :tree.time,
                 'place':tree.GetName()})
    return

def getDataEvent(tree, data, index):
    if (tree.GetEntry(index) <= 0): return # no data, shouldn't happen
    if not cut(tree.pid, (-13, -11, 11, 13)): return
    key = tree.eventNo
    value = {'PID'  :tree.pid,
             'pos'  :(tree.posX, tree.posY, tree.posZ),
             'time' :tree.time,
             'place':tree.GetName()}
    if data.has_key(key): # if the event exists append to it otherwise create it
        data[key].append(value)
    else:
        data[key] = [value,]
    return


file_in = TFile('../output/truth_out.root', 'READ')
treeA = file_in.Get("monA")
treeB = file_in.Get("monB")

nentriesA = treeA.GetEntries()
nentriesB = treeB.GetEntries()

# data=[] # list of all entries of interest
dataEvent={}

# loop over all entries
for index in range(nentriesA if nentriesA>nentriesB else nentriesB):
    # boiler plate checks that the data exists
    if index < nentriesA: 
        # save the information from monitorA
        # getDataFlat(treeA, data, index)
        getDataEvent(treeA, dataEvent, index)
            
    if index < nentriesB: 
        # also save the information from monitor B
        # getDataFlat(treeB, data, index)
        getDataEvent(treeB, dataEvent, index)
        
# data should now have all the muons and electrons from A&B

# timeOrderedData = sorted(data, key = lambda s: s['time']) 

eventOD = OrderedDict(dataEvent)

# print eventOD[eventOD.keys()[10]] #print the first item

muon_count = 0
stopped_muons = 0
non_stop_muons = 0

for eventID in eventOD:
    event = eventOD[eventID] # alias
    if len(eventOD[eventID]) < 2:
        # Don't just skip empty events, still cound towards number of muons
        if (event[0]['PID'] == 13) or  (event[0]['PID'] == -13): 
            muon_count+=1 
        continue
        
    event = sorted(event, key=lambda s: s['time']) # time sort the event
    for i in event: print i
    print '*'*40
    possible_hit = False
    stopped = False
    for hit in event:
        if (abs(hit['PID'])==13): muon_count += 1
        if possible_hit and (abs(hit['PID'])==13) and (hit['place']=='monA'): 
            # muon didn't stop
            non_stop_muons += 1
            stopped = False
        elif (abs(hit['PID'])==13) and (hit['place']=='monB'): 
            possible_hit = True
            stopped = True
    if possible_hit and stopped: stopped_muons += 1
    
print "total events", len(eventOD)
print "muon count", muon_count
print "stopped_muons", stopped_muons
print "non_stop_muons", non_stop_muons
    
    
    
    
    
    
    