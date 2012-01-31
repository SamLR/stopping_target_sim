//
//  STSmartFile.cc
//  stopping_target_sim
//
//  Created by Sam Cook on 30/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "STSmartTFile.hh"

map<G4String, STSmartTFile*> STSmartTFile::mFileMap = map<G4String, STSmartTFile*>();

STSmartTFile::STSmartTFile(): mPtrCount(1) {;}
STSmartTFile::STSmartTFile(TString filename, TString options)
    :TFile(filename, options), mPtrCount(1) 
{;}

STSmartTFile::~STSmartTFile() {;}

void STSmartTFile::close()
{
    --mPtrCount;
    if (mPtrCount==0) 
    {
        this->Write();
        delete this;
    }
}

STSmartTFile* STSmartTFile::getTFile(G4String filename, G4String options)
{
    map<G4String, STSmartTFile*>::iterator iter;
    iter = mFileMap.find(filename);
    if (iter == mFileMap.end()) 
    {
        // new file
        STSmartTFile* ptr = new STSmartTFile(filename.c_str(), options.c_str());
        if (!ptr->IsOpen()) 
        {
            G4cout << "\n[ERROR] file "<< filename <<" not opened, exiting"<< G4endl;
            exit(1);
        }
        mFileMap[filename] = ptr;
        return ptr;
    } else
    { 
        STSmartTFile* ptr = iter->second;
        ptr->incPtrCount();
        return ptr;
    }
    
}



