//
//  STSmartFile.hh
//  stopping_target_sim
//
//  Created by Sam Cook on 30/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef STSmartFile_hh
#define STSmartFile_hh 1

#include "globals.hh"
#include "TFile.h"
#include <map>

using namespace std;

class STSmartTFile: public TFile
{
public:
    static STSmartTFile* getTFile(G4String, G4String); // returns the smart file of given name with options
    
    void close();
    
//    void addTree(G4String);
private:
    STSmartTFile();
    STSmartTFile(TString, TString);
    ~STSmartTFile();
    
    G4int getPtrCount(){return mPtrCount;}
    void incPtrCount(){++mPtrCount;}
    void decPtrCount(){--mPtrCount;}
    
    G4int mPtrCount;
    
    static map<G4String, STSmartTFile*> mFileMap; // filename: file ptr pairs
    
};



#endif
