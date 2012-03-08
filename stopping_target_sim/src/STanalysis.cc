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
    mFile = STSmartTFile::getTFile(filename.c_str(), "RECREATE");
    
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
    mTree->Branch("trackID" , &mTrackID, "trackID/I");
    mTree->Branch("ParentID" , &mParentID, "ParentID/I");
    mTree->Branch("pid" , &mPID, "PID/I");
    mTree->Branch("posX", &mX, "posX/F");
    mTree->Branch("posY", &mY, "posY/F");
    mTree->Branch("posZ", &mZ, "posZ/F");
    mTree->Branch("momX", &mX, "momX/F");
    mTree->Branch("momY", &mY, "momY/F");
    mTree->Branch("momZ", &mZ, "momZ/F");
    mTree->Branch("time", &mT, "time/F");
}

void STanalysis::addHit(G4int eventNo, G4int pid, G4int parentID, G4int trackID,
                        G4float* position, G4float* momentum, G4float time)
{
    mEvent = eventNo;
    mPID = pid;
    mParentID = parentID;
    mTrackID = trackID;
    mX = position[0];
    mY = position[1];
    mZ = position[2];
    mPx = momentum[0];
    mPy = momentum[1];
    mPz = momentum[2];
    mT = time;
    mTree->Fill();
}

void STanalysis::addHit(G4int eventNo,G4int pid, G4int parentID, G4int trackID,
                        G4ThreeVector position, G4ThreeVector momentum, G4float time)
{
    G4float positionT[3] = {position.x(), position.y(), position.z()};
    G4float momentumT[3] = {momentum.x(), momentum.y(), momentum.z()};
    addHit(eventNo, pid, parentID, trackID, positionT, momentumT, time);
}

void STanalysis::update()
{
//    mFile->Write();
}