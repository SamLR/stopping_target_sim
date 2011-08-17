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
G4int STbeamReadin::mPtrCount = 0;

STbeamReadin::STbeamReadin(): 
    mCurrentParticle(0),
    // zOffset = (g4beamline - counter:beamend separation)
    xOffset(0),     yOffset(0),     zOffset(-(3901.18+100)*mm), 
    maxX( 1000*mm), maxY( 1000*mm), maxZ( 1000*mm),
    minX(-1000*mm), minY(-1000*mm), minZ(-1000*mm)
{;}

void STbeamReadin::destroy()
{
    delete mInstancePtr;
    mInstancePtr = NULL;
}

STbeamReadin::~STbeamReadin()//{;}
{
    --mPtrCount;
    if (mPtrCount <= 0)
    {
//        destroy();
    }
}

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
    ++mPtrCount;
    return mInstancePtr;
}

void STbeamReadin::initialise(G4String file)
{
    ifstream fileIn (file);
    
    if (fileIn.is_open())
    {
        while (fileIn.good())
        {
            G4float eventNo, pid;
            G4float pos[3];
            G4float mom[3];
            inputParticle currentParticle;
            
            fileIn >> eventNo >> pid 
                   >> pos[0] >> pos[1] >> pos[2] 
                   >> mom[0] >> mom[1] >> mom[2];
            
            // some of the PIDs given are in valid; remove them
            if (pid>1000000000) continue;
            // check for charged particles pi, mu, e or p. 
            G4bool charged = (pid == -211 || pid == -13 || pid == -11) || 
                             (pid ==  211 || pid ==  13 || pid ==  11) ||
            (pid == 2212);

            if (!charged) continue;
            
            pos[0] += xOffset;// move within world volume
            pos[1] += yOffset;
            pos[2] += zOffset; 
            
            if (not checkbounds(pos)) continue;
            
            currentParticle.status = 1; 
            currentParticle.PDG_id = (int) pid;
            currentParticle.position = G4ThreeVector(pos[0], pos[1], pos[2]);
            currentParticle.momentum = G4ThreeVector(mom[0], mom[1], mom[2]);
            mParticleVec.push_back(currentParticle);
        }
    }
    fileIn.close();
}
