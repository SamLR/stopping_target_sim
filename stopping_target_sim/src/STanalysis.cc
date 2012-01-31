/*
 *  STanalysis.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 25/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#include "STanalysis.hh"


STanalysis::STanalysis(): mFile(NULL), mTree(NULL) {;}

STanalysis::STanalysis(G4String filename, G4String treename)
{
    initialise(filename, treename); // open the file and make a tree
}

STanalysis::~STanalysis() 
{
    close();
}

void STanalysis::close()
{
    mFile->close();
//    if (mFile->IsOpen())
//    {
//        update();
//        G4cout << "closing " << mFile->GetName()<< G4endl;
//        mFile->Close();
//        delete mFile;
//    }
}

void STanalysis::initialise(G4String  filename, G4String treename)
{
//    mFile = new TFile(filename.c_str(), "UPDATE");
    mFile = STSmartTFile::getTFile(filename.c_str(), "UPDATE");
    
    if (!mFile) {
        G4cout << "Error: opening " << filename << " for writing" << G4endl;
        exit(1);
    }
    
    addTree(treename);
}

void STanalysis::addTree(G4String treename)
{
    mTree = new TTree(treename,treename);
    mTree->Branch("eventNo", &mEvent, "eventNo/I");
    mTree->Branch("pid" , &mPID, "PID/I");
    mTree->Branch("posX", &mX, "posX/F");
    mTree->Branch("posY", &mY, "posY/F");
    mTree->Branch("posZ", &mZ, "posZ/F");
    mTree->Branch("time", &mT, "time/F");
}

void STanalysis::addHit(G4int eventNo, G4int pid,  G4float* position, G4float time)
{
    mEvent = eventNo;
    mPID = pid;
    mX = position[0];
    mY = position[1];
    mZ = position[2];
    mT = time;
    mTree->Fill();
}

void STanalysis::addHit(G4int eventNo,G4int pid,  G4ThreeVector position, G4float time)
{
    G4float positionT[3] = {position.x(), position.y(), position.z()};
    addHit(eventNo, pid, positionT, time);
}

void STanalysis::update()
{
//    mFile->Write();
}