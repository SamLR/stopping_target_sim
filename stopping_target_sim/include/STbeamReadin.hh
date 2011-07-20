/*
 *  STg4readin.h
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 04/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#ifndef STBEAMREADIN_HH
#define STBEAMREADIN_HH

#include "globals.hh"
#include <vector>

#include "G4ThreeVector.hh"

using namespace std;

struct inputParticle {
    G4int status;
    G4int PDG_id;
    G4ThreeVector position;
    G4ThreeVector momentum;
};

class STbeamReadin  
{
public:
    inputParticle next();
    static STbeamReadin* getPointer(G4String file);
    ~STbeamReadin();
    G4int inline getMaxParticles() {return mParticleVec.size();}
    
private:
    STbeamReadin();
    void initialise(G4String file);
    static STbeamReadin* mInstancePtr;
    vector<inputParticle> mParticleVec;
    G4int mCurrentParticle;
    G4float* transformToLocal (G4float* in);
    
};


#endif 