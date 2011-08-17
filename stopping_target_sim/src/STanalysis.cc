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
G4int STanalysis::mPtrCount = 0;

STanalysis* STanalysis::getPointer(G4String filename)
{
    if (mInstancePtr == NULL)
    {
        mInstancePtr = new STanalysis();
        mInstancePtr->initialise(filename);
    }
    ++mPtrCount;
    return mInstancePtr;
}

STanalysis::STanalysis(): mX(0), mY(0), mZ(0), mT(0) {;}

void STanalysis::destroy()
{
    delete mInstancePtr;
    mInstancePtr = NULL;
    mPtrCount = 0;
}

STanalysis::~STanalysis()// {;}
{
    --mPtrCount;
    if (mPtrCount <= 0)
    {
        if (mFile) update();
        mFile->Close();
        delete mFile;
        delete mTree;
        mFile = 0;
        mTree = 0;
//        destroy();
    };
}

void STanalysis::initialise(G4String filename)
{
    mFile = new TFile(filename.c_str(), "RECREATE");
    if (!mFile) {
        G4cout << "Error: opening " << filename << " for writing" << G4endl;
        exit(1);
    }
    
    mTree = new TTree("MPPC_Data","hits on MPPCS");
    mTree->Branch("posX", &mX, "posX/F");
    mTree->Branch("posY", &mY, "posY/F");
    mTree->Branch("posZ", &mZ, "posZ/F");
    mTree->Branch("time", &mT, "time/F");
}


void STanalysis::addHit(G4float* position, G4float time)
{

    
    mX = position[0];
    mY = position[1];
    mZ = position[2];
    mT = time;
    mTree->Fill();
}

void STanalysis::update()
{
    mFile->Write();
}