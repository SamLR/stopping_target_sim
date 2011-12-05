/*
 *  STanalysis.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 25/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#include "STanalysis.hh"

STanalysis* STanalysis::mInstancePtr = NULL;
int STanalysis::mPtrCount = 0;

STanalysis::STanalysis(): mFile(NULL), mTree(NULL) {;}

STanalysis::STanalysis(G4String filename): mFile(NULL), mTree(NULL)
{
    initialise(filename);
}

STanalysis::~STanalysis() {;}

void STanalysis::close(G4bool override)
{
    --mPtrCount;
    if ((mPtrCount <= 0 || override) && mInstancePtr)
    {
        mFile->Close();
        delete mFile;
        destroy();
    }
}

void STanalysis::destroy()
{
    mInstancePtr = NULL;
    mPtrCount = 0;
}

STanalysis* STanalysis::getPointer(G4String filename)
{
    if (!mInstancePtr)
    {
        mInstancePtr = new STanalysis(filename);
    }
    ++mPtrCount;
    return mInstancePtr;
}

STanalysis* STanalysis::getInitdPointer()
{
    if (!mInstancePtr) 
    {
        G4cout << "no instance initialised exiting"<<G4endl;
        exit(1);
    } else {
        ++mPtrCount;
        return mInstancePtr;
    }
}

void STanalysis::initialise(G4String filename)
{
    mFile = new TFile(filename.c_str(), "RECREATE");
    
    if (!mFile) {
        G4cout << "Error: opening " << filename << " for writing" << G4endl;
        exit(1);
    }
    
    mTree = new TTree("MPPC_Data","hits on MPPCS");
    mTree->Branch("eventNo", &mEvent, "eventNo/I");
    
    mTree->Branch("posX", &mX, "posX/F");
    mTree->Branch("posY", &mY, "posY/F");
    mTree->Branch("posZ", &mZ, "posZ/F");
    mTree->Branch("time", &mT, "time/F");
}


void STanalysis::addHit(G4int eventNo, G4float* position, G4float time)
{
    mEvent = eventNo;
    mX = position[0];
    mY = position[1];
    mZ = position[2];
    mT = time;
    mTree->Fill();
}

void STanalysis::addHit(G4int eventNo, G4ThreeVector position, G4float time)
{
    G4float positionT[3] = {position.x(), position.y(), position.z()};
    addHit(eventNo, positionT, time);
}

void STanalysis::update()
{
    mFile->Write();
}