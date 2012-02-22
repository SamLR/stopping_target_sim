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

void STSmartTFile::forceClose()
{
    // automatically closes all open files ignoring number of active pointers
    cout <<endl << endl << "WARNING: force closing all open root files"<< endl;
    map<G4String, STSmartTFile*>::iterator iter;
    for (iter = mFileMap.begin(); iter != mFileMap.end(); ++iter) 
    {
        iter->second->Write();
        iter->second->Close();
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



