//
//  STMonitorSD.cc
//  stopping_target_sim
//
//  Created by Sam Cook on 30/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "STMonitorSD.hh"
#include "STanalysis.hh"
#include "G4SDManager.hh"

STMonitorSD::STMonitorSD(G4String name, G4String filename, G4String treename): 
    G4VSensitiveDetector(name), mEventNumber(0) 
{
    G4String HCname = name + "CounterCollection";
    collectionName.insert(HCname);
    mAnalysis = new STanalysis(filename, treename);
}

STMonitorSD::~STMonitorSD()
{
    mAnalysis->update();
    delete mAnalysis;
}

void STMonitorSD::Initialize(G4HCofThisEvent *pThisHC)
{
    ++mEventNumber;
    counterCollection = new STMonitorHitsCollection(SensitiveDetectorName, 
                                                    collectionName[0]);
    static G4int HCID = -1; // test if this gets assigned on subsequent calls
    if (HCID < 0) 
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    pThisHC->AddHitsCollection(HCID, counterCollection);
}

void STMonitorSD::EndOfEvent(G4HCofThisEvent *pThisHC) 
{
    G4int nHits = counterCollection->entries();
    
    for (int i = 0; i<nHits; ++i) 
    {
        G4int pid = (*counterCollection)[i]->GetPID();
        G4int parent = (*counterCollection)[i]->GetParentID();
        G4int track = (*counterCollection)[i]->GetTrackID();
        G4ThreeVector position = (*counterCollection)[i]->GetPos();
        G4ThreeVector momentum = (*counterCollection)[i]->GetMom();
        G4float time = (*counterCollection)[i]->GetTime();
        mAnalysis->addHit(mEventNumber, pid, parent, track, position, momentum, time);
    }
//    mAnalysis->update(); 
}

G4bool STMonitorSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // add checks that first hit etc
    G4StepStatus stepStatus = aStep->GetPreStepPoint()->GetStepStatus();
    if (not stepStatus == fGeomBoundary) return false;
    
    G4int pid = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
    
    if (pid == 0) return false; // ignore optical photons
    
    STMonitorHit *newHit = new STMonitorHit();
    newHit->SetEvent(mEventNumber);    
    newHit->SetPid(pid);
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
    newHit->SetMom( aStep->GetPostStepPoint()->GetMomentum()); // momentum
    newHit->SetTrackID(aStep->GetTrack()->GetTrackID()); // ID for this track/particle
    newHit->SetParentID(aStep->GetTrack()->GetParentID());
    counterCollection->insert(newHit);
    return true;
}