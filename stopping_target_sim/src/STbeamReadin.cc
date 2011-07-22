/*
 *  STbeamReadin.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 04/07/2011.
 *
 */

#include "STbeamReadin.hh"

#include <fstream>
#include <iostream>
//#include <string> // check to see if G4String supports this
#include <string.h>
#include <ctype.h>

#include "string_conv.hh" // tools for converting strings to numbers

using namespace std; // for files etc

STbeamReadin* STbeamReadin::mInstancePtr = NULL;

STbeamReadin::STbeamReadin(){;}

STbeamReadin::~STbeamReadin(){;}

inputParticle STbeamReadin::next()
{
    if (mCurrentParticle == mParticleVec.size())
    {
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
            getline(fileIn, line);
            
            // check for empty lines
            if (line.length() < 1) break;
            
            char cstr[150];
            strcpy(cstr, line.c_str());
            
            G4int column = 0;
            G4int eventNo, pid;
            G4float pos_tmp[3];
            G4float mom[3];
            inputParticle currentParticle;
            
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
                        pos_tmp[0] = atof(pcr)*mm;
                        break;
                    case 3: // pos y
                        pos_tmp[1] = atof(pcr)*mm;
                        break;
                    case 4: // pos z
                        pos_tmp[2] = atof(pcr)*mm;
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
            if (pid>1000000000) continue;
            
            G4float* pos = transformToLocal(pos_tmp);
            // if the position is out of the world volume.....
            if (!pos) continue; 
            currentParticle.status = 1; 
            currentParticle.PDG_id = pid;//string_to_int(pid);
            currentParticle.position = G4ThreeVector(pos[0], pos[1], pos[2]);
            currentParticle.momentum = G4ThreeVector(mom[0], mom[1], mom[2]);
            mParticleVec.push_back(currentParticle);
            if (pos) delete[] pos; // clean up
        }
    }
}

// utility function to convert from g4beamline to local co-ordinates
G4float* STbeamReadin::transformToLocal (G4float* in)
{
    // this should probably be 'get'ed
    G4float max_extent = 1.0*m; // maximum co-ordinate value w/ in the world
    G4float* out = new G4float[3];
    
    out[0] -= 2757.60*mm; // magic transformation number
    
    for (int i = 0; i < 3; ++i) 
    { // check that all positions are still within the world
        if (out[i] > max_extent || out[i] < -max_extent) 
        {
            delete[] out;
            out = NULL;
            break;
        }
    }
    return out;
}


