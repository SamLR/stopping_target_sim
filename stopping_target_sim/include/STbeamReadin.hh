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
#include <fstream>
#include <iostream>
#include <vector>

#include "G4ThreeVector.hh"

// REFACTOR THIS DOESNT NEED TO BE A SINGLETON

using namespace std;

struct inputParticle { // one inputParticle ~ 56B (4 + 4 + 3*8 + 3*8)
    G4int status;
    G4int PDG_id;
    G4ThreeVector position;
    G4ThreeVector momentum;
    G4float time_offset; // offset from when the proton was produced (in G4BL)
};

//static const G4int mMaxParticlesInArray = 1000;
	
class STbeamReadin  
{
public:
    inputParticle next();
    static STbeamReadin* getPointer(G4String file);
    static void close();
//    G4int inline getNParticlesInArray() {return mMaxParticlesInArray;}
    
    G4int inline getNParticles() {return mNParticles;}
    G4int inline getMaxParticles() {return mNParticles;}
    
private:
    STbeamReadin();
    STbeamReadin(G4String file);
    ~STbeamReadin();
    void initialise(G4String file);
    void loadParticles(ifstream*);
//        void loadParticles();
    static STbeamReadin* mInstancePtr;

//    ifstream* mFileIn;
    
    vector<inputParticle> mParticleVec;
//    inputParticle mParticles [mMaxParticlesInArray]; // TODO should probably be defined somewhere
//    inputParticle* mCurrentParticle;
    G4int mCurrentParticle;
    G4int mNParticles; // total number of particles in the array
    
    G4float xOffset, yOffset, zOffset; // all other co-ordinates should map 1:1
    G4float maxX, maxY, maxZ;
    G4float minX, minY, minZ;
    G4float proton_rate; // multiplier for eventIDs to convert to 'real' times
    
    inline G4bool checkbounds(G4float position[3])
    {
        return (minX < position[0] && position[0] < maxX) &&
               (minY < position[1] && position[1] < maxY) &&
               (minZ < position[2] && position[2] < maxZ);
    }
    
};


#endif 