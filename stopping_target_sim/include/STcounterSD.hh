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
#include "STcounterHit.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"


class G4Step;
class G4HCofThisEvent;

class STcounterSD : public G4VSensitiveDetector 
{
public:
    STcounterSD(G4String name);
    ~STcounterSD();
    
    // needed to set up Root
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    
    // required method
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);

    
private:
    // count the hits
    static G4int totalCount;
    
    // Hits collection
    STcounterHitsCollection* counterCollection;
};

# endif