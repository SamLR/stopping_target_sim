/*
 *  STg4readin.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 04/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#include "STbeamReadin.hh"

#include <fstream>
#include <iostream>
//#include <string> // check to see if G4String supports this
#include <string.h>

#include "string_conv.hh" // tools for converting strings to numbers

using namespace std; // for files etc

STbeamReadin* STbeamReadin::mInstancePtr = NULL;

STbeamReadin::STbeamReadin(){;}

STbeamReadin::~STbeamReadin(){;}

inputParticle STbeamReadin::next()
{
    if (mCurrentParticle == mParticleVec.size())
    {
        G4cout << "End of Primaries" << G4endl;
        inputParticle error;
        error.status = -1;
        return error;
    }
    return mParticleVec[mCurrentParticle++];
}

STbeamReadin* STbeamReadin::getPointer(G4String file)
{
    if (mInstancePtr==NULL) 
    {
        mInstancePtr = new STbeamReadin();
        mInstancePtr->initialise(file);
    }
    return mInstancePtr;
}

void STbeamReadin::initialise(G4String file)
{
    ifstream fileIn (file);
    G4String line; 
    mCurrentParticle = 0;
    
    if (fileIn.is_open())
    {
        while (fileIn.good())
        {
            G4int column = 0;
            G4int eventNo, pid;
            G4float pos[3];
            G4float mom[3];
            inputParticle currentParticle;
            
            getline(fileIn, line);
            
            char cstr[150];
            strcpy(cstr, line.c_str());
            char* pcr = strtok(cstr, " ");
            
            while (pcr != NULL) 
            {
                switch (column) 
                {
                    case 0: // event Number
                        eventNo = atoi(pcr);
                        break;
                    case 1: // PDG id
                        pid = atoi(pcr);
                        break;
                    case 2: // pos x
                        pos[0] = atof(pcr)*mm;
                        break;
                    case 3: // pos y
                        pos[1] = atof(pcr)*mm;
                        break;
                    case 4: // pos z
                        pos[2] = atof(pcr)*mm;
                        break;
                    case 5: // mom x
                        mom[0] = atof(pcr)*mm;
                        break;
                    case 6: // mom y
                        mom[1] = atof(pcr)*mm;
                        break;
                    case 7: // mom z
                        mom[2] = atof(pcr)*mm;
                        break;
                    default:
                        break;
                }
                pcr = strtok (NULL, " ");
                ++column;
            }
            // some of the PIDs given are in valid; remove them
            if (pid>1000000000) {continue;}
            currentParticle.status = 1; 
            currentParticle.PDG_id = pid;//string_to_int(pid);
            currentParticle.position = G4ThreeVector(pos[0], pos[1], pos[2]);
            currentParticle.momentum = G4ThreeVector(mom[0], mom[1], mom[2]);
            mParticleVec.push_back(currentParticle);
        }
    }
}


