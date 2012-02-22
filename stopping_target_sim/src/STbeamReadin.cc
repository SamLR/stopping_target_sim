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

STbeamReadin::STbeamReadin() {;}
STbeamReadin::STbeamReadin(G4String file): 
    mCurrentParticle(0), mParticleVec(0),
    // xOffset(0),     yOffset(0),     zOffset(-2757*mm), // average zoffset 
    xOffset(0),     yOffset(0),     zOffset(-3901*mm), // average zoffset 
    maxX( 1000*mm), maxY( 1000*mm), maxZ( 1000*mm),
    minX(-1000*mm), minY(-1000*mm), minZ(-1000*mm)
{
    initialise(file);
}

void STbeamReadin::close()
{
    mInstancePtr = NULL;
}

STbeamReadin::~STbeamReadin() {;}

inputParticle STbeamReadin::next()
{
    if (mCurrentParticle == mNParticles) // out of particles
    {
        G4cout << "out of particles" << G4endl;
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
        mInstancePtr = new STbeamReadin(file);
    }
    return mInstancePtr;
}

void STbeamReadin::initialise(G4String file)
{
    ifstream* fileIn = new ifstream();
    fileIn->open(file);
         
    if (fileIn->is_open())
    {
//        loadParticles();
        loadParticles(fileIn);
    } else {
        G4cout << "ERROR: file <" << file << "> not opened" << G4endl;
        exit(1);
    }
}

//void STbeamReadin::loadParticles()
void STbeamReadin::loadParticles(ifstream* fileIn)
{
    while (fileIn->good())
    {
        G4float eventNo, pid, pos[3], mom[3];
        
        (*fileIn) >> eventNo >> pid 
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
        inputParticle* currentParticle = new inputParticle();
        
        currentParticle->status = 1; 
        currentParticle->PDG_id = (int) pid;
        currentParticle->position = G4ThreeVector(pos[0], pos[1], pos[2]);
        currentParticle->momentum = G4ThreeVector(mom[0], mom[1], mom[2]);
        
        mParticleVec.push_back(*currentParticle);
    }
    
    mNParticles = mParticleVec.size();
    fileIn->close();
    fileIn = NULL;
    
}
