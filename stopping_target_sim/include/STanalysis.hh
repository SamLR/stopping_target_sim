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

// ideally this should be set up so that it uses a single file
// but creates a separate tree for each MPPC

class STanalysis 
{
public:    
    static void destroy();
    static STanalysis* getPointer(G4String filename);
    inline static STanalysis* getPointer() {return mInstancePtr;}
    
    ~STanalysis();
    void addHit(G4float* position, G4float time);
    void update();
    
private:
    STanalysis();
    
    void initialise(G4String filename);
    
    static STanalysis* mInstancePtr;
    
    static G4int mPtrCount;
    TFile* mFile;
    TTree* mTree;
    Double_t mX, mY, mZ, mT; // variables that will write to the tree

};

#endif