/*
 *  STg4readin.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 04/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#include "STbeamReadin.hh"

#include "G4ThreeVector.hh"

struct primaryParticle {
    G4int PDG_id;
    G4ThreeVector position;
    G4ThreeVector momentum;
}

STbeamReading* STbeamReadin::mInstancePtr = 0;

STbeamReadin::STbeamReadin(){;}

STbeamReadin::~STbeamReadin(){;}

void STbeamReadin::next()
{
    ;
}

STbeamReadin* STbeamReadin::getPointer(G4String file)
{
    if (mInstancePtr==0) {
        mInstancePtr = new STbeamReadin();
    }
    return mInstancePtr;
}

void STbeamReadin::initialise(G4String file)
{
    ;
    
}


