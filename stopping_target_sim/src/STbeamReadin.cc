/*
 *  STg4readin.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 04/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#include "STbeamReadin.hh"

#include <fstream>
#include <iostream>
//#include <string> // check to see if G4String supports this

//#include "string_conv.hh" // tools for converting strings to numbers

using namespace std; // for files etc

STbeamReadin* STbeamReadin::mInstancePtr = NULL;

STbeamReadin::STbeamReadin(){;}

STbeamReadin::~STbeamReadin(){;}

inputParticle STbeamReadin::next()
{
    if (mCurrentParticle == mParticleVec.size())
    {
        G4cout << "End of Primaries" << G4endl;
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
    G4String line; 
    mCurrentParticle = 0;
    
    if (fileIn.is_open())
    {
        while (fileIn.good())
        {
            // read in
            G4int eventNo; // to be discarded
            G4int pid; 
            G4float posx, posy, posz;
            G4float momx, momy, momz;
            inputParticle currentParticle;
            
            fileIn >> eventNo >> pid 
                   >> posx >> posy >> posz
                   >> momx >> momy >> momz;
            
            currentParticle.status = 1; 
            currentParticle.PDG_id = pid;
            currentParticle.position = G4ThreeVector(posx, posy, posz);
            currentParticle.momentum = G4ThreeVector(momx, momy, momz);
            mParticleVec.push_back(currentParticle);
        }
    }
}


