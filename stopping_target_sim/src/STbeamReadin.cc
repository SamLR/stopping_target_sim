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

STbeamReadin::~STbeamReadin() {;}

inputParticle STbeamReadin::next()
{
    if (mNParticlesRemaining == 0)
    {
        if (mFileIn->is_open())
        {
            loadParticles();
        } else {
            G4cout << "out of particles" << G4endl;
            inputParticle error;
            error.status = 0;
            return error;
        }
    }
    inputParticle res = (*mCurrentParticle);
    ++mCurrentParticle;
    --mNParticlesRemaining;
    return res;
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
    mFileIn = new ifstream();
    mFileIn->open(file);
    
    if (mFileIn->is_open())
    {
        loadParticles();
    } else {
        G4cout << "ERROR: file <" << file << "> not opened" << G4endl;
        exit(1);
    }
}

void STbeamReadin::loadParticles()
{
    G4int particle_count = 0;
    mCurrentParticle = mParticles;
    
    while (mFileIn->good() && particle_count < mMaxParticlesInArray)
    {
        G4float eventNo, pid, pos[3], mom[3];
        inputParticle currentParticle;
        
        (*mFileIn) >> eventNo >> pid 
                   >> pos[0] >> pos[1] >> pos[2] 
                   >> mom[0] >> mom[1] >> mom[2];
        
        // some of the PIDs given are invalid; remove them
        // check for charged particles pi (211), mu (13), e(11) or p(2212). 
        G4bool charged = (pid == -211 || pid == -13 || pid == -11) ||  
                         (pid ==  211 || pid ==  13 || pid ==  11) ||
                         (pid == 2212);
        
        if (!charged) continue;
        
        pos[0] += xOffset;// move within world volume
        pos[1] += yOffset;
        pos[2] += zOffset; 
        
        if (not checkbounds(pos)) continue; // check in world volume
        
        currentParticle.status = 1; 
        currentParticle.PDG_id = (int) pid;
        currentParticle.position = G4ThreeVector(pos[0], pos[1], pos[2]);
        currentParticle.momentum = G4ThreeVector(mom[0], mom[1], mom[2]);
        
        (*mCurrentParticle) = currentParticle; // enter into array
        ++mCurrentParticle;
        ++particle_count;
    }
    
    mCurrentParticle = mParticles; // reset pointer to first particle
    mNParticles = particle_count;
    mNParticlesRemaining = particle_count;
    if (!mFileIn->good()) // if the file has run out of entries close it
    {
        mFileIn->close();
        mFileIn = NULL;
    }
    
}
