//
//  STcounterSD.h
//  stopping_target_sim
//
//  Created by Sam Cook on 20/06/2011.
//  Copyright 2011 UCL. All rights reserved.
//


class STcounterSD : public G4VSensitiveDetector{
    
    
public:
    STcounterSD();
    ~STcounterSD();
    
    // required method
    G4bool  ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
    
    // needed for root and generally useful
    void Initialize(G4HCofThisEvent*);
    
    
    
};