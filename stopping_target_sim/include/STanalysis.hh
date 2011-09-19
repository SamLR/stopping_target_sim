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
#include "TFile.h"
#include "TTree.h"

#include "G4ThreeVector.hh"

// ideally this should be set up so that it uses a single file
// but creates a separate tree for each MPPC

class STanalysis 
{
public:    
    
    void addHit(G4int eventNo, G4float* position, G4float time);
    void addHit(G4ThreeVector position, G4float time);
    void update();
    void close(G4bool override = FALSE);
    static STanalysis* getPointer(G4String filename="out.root");
    static STanalysis* getInitdPointer();
    
private:
    STanalysis();
    ~STanalysis();
    STanalysis(G4String filename);
    
    static void destroy();
    
    void initialise(G4String filename);
    
    static STanalysis* mInstancePtr;
    static int mPtrCount;
    
    TFile* mFile;
    TTree* mTree;
    Int_t mEvent;
    Float_t mX, mY, mZ, mT; // variables that will write to the tree
};

#endif