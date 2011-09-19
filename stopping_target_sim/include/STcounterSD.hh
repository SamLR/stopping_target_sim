//
//  STcounterSD.h
//  stopping_target_sim
//
//  Created by Sam Cook on 20/06/2011.
//  Copyright 2011 UCL. All rights reserved.
//

#ifndef STcounterSD_HH
#define STcounterSD_HH 1

#include "STcounterSD.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "STanalysis.hh"
#include "STcounterHit.hh"

class STcounterSD : public G4VSensitiveDetector 
{
public:
    STcounterSD(G4String);
    ~STcounterSD();
    
    // required methods
    void Initialize(G4HCofThisEvent *pThisHC);
    void EndOfEvent(G4HCofThisEvent *pThisHC);
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
    
private:
    
//    STanalysis* mAnalysis; // should this be here? 
    STcounterHitsCollection *counterCollection;
};

# endif