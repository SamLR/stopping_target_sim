//
//  STMonitorSD.hh
//  stopping_target_sim
//
//  Created by Sam Cook on 30/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef STMonitorSD_hh
#define STMonitorSD_hh

#include "STMonitorSD.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "STanalysis.hh"
#include "STMonitorHit.hh"

class STMonitorSD : public G4VSensitiveDetector 
{
public:
    STMonitorSD(G4String, G4String, G4String);
    ~STMonitorSD();
    
    // required methods
    void Initialize(G4HCofThisEvent *pThisHC);
    void EndOfEvent(G4HCofThisEvent *pThisHC);
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
    
private:
    
    G4int mEventNumber;
    STMonitorHitsCollection *counterCollection;
    STanalysis* mAnalysis;
};

#endif
