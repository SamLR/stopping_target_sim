/*
 *  STbeamReadin.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 04/07/2011.
 *
 */

#include "STbeamReadin.hh"

#include <fstream>
#include <iostream>

using namespace std; // for files etc

STbeamReadin* STbeamReadin::mInstancePtr = NULL;

STbeamReadin::STbeamReadin(){;}

STbeamReadin::~STbeamReadin(){;}

inputParticle STbeamReadin::next()
{
    if (mCurrentParticle == mParticleVec.size())
    {
        inputParticle error;
        error.status = -1;
        return error;
    }
    return mParticleVec[mCurrentParticle++];
}

STbeamReadin* STbeamReadin::getPointer(G4String file)
{
    if (mInstancePtr==NULL) 
    {
        mInstancePtr = new STbeamReadin();
        mInstancePtr->initialise(file);
    }
    return mInstancePtr;
}

void STbeamReadin::initialise(G4String file)
{
    ifstream fileIn (file);
    mCurrentParticle = 0;
    
    if (fileIn.is_open())
    {
        while (fileIn.good())
        {
            G4float eventNo, pid;
            G4float pos_tmp[3];
            G4float mom[3];
            inputParticle currentParticle;
            
            fileIn >> eventNo >> pid 
                   >> pos_tmp[0] >> pos_tmp[1] >> pos_tmp[2] 
                   >> mom[0] >> mom[1] >> mom[2];
            
            // some of the PIDs given are in valid; remove them
            if (pid>1000000000) continue;
            
            G4float* pos = transformToLocal(pos_tmp);
            // if the position is out of the world volume.....
            if (!pos) continue; 
            currentParticle.status = 1; 
            currentParticle.PDG_id = (int) pid;
            currentParticle.position = G4ThreeVector(pos[0], pos[1], pos[2]);
            currentParticle.momentum = G4ThreeVector(mom[0], mom[1], mom[2]);
            mParticleVec.push_back(currentParticle);
            if (pos) delete[] pos; // clean up
        }
    }
    fileIn.close();
}

// utility function to convert from g4beamline to local co-ordinates
G4float* STbeamReadin::transformToLocal (G4float* in)
{
    // this should probably be 'get'ed
    G4float max_extent = 1.0*m; // maximum co-ordinate value w/ in the world
    G4float* out = new G4float[3];
    
    out[0] -= 2757.60*mm; // magic transformation number
    
    for (int i = 0; i < 3; ++i) 
    { // check that all positions are still within the world
        if (out[i] > max_extent || out[i] < -max_extent) 
        {
            delete[] out;
            out = NULL;
            break;
        }
    }
    return out;
}


