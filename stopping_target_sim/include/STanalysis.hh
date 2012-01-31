/*
 *  analysis.h
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 25/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#ifndef STANALYSIS_HH
#define STANALYSIS_HH 1

#include "globals.hh"
#include "TROOT.h"
//#include "TFile.h"
#include "STSmartTFile.hh"
#include "TTree.h"

#include "G4ThreeVector.hh"

// ideally this should be set up so that it uses a single file
// but creates a separate tree for each MPPC

class STanalysis 
{
public:    
    
    // add hit (evet, pid, position, time)
    void addHit(G4int, G4int, G4float*, G4float);
    void addHit(G4int, G4int, G4ThreeVector, G4float);
    void update();
    
    STanalysis();
    STanalysis(G4String, G4String);
    ~STanalysis();
    
private:
    
    void close();
    
    void initialise(G4String, G4String);
    void addTree(G4String);
    
    static STanalysis* mInstancePtr;
    static int mPtrCount;
    
    G4String mFileName;
//    TFile* mFile;
    STSmartTFile* mFile;
    TTree* mTree;
    Int_t mEvent;
    Int_t mPID;
    Float_t mX, mY, mZ, mT; // variables that will write to the tree
};

#endif