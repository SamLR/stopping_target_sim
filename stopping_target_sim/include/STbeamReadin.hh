/*
 *  STg4readin.h
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 04/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#ifndef STBEAMREADIN_HH
#define STBEAMREADIN_HH

#include "globals.hh"

class STbeamReadin : public  
{
public:
    void next();
    static STbeamReadin* getPointer(G4String file);
    ~STbeamReadin();
    
private:
    STbeamReadin();
    void initialise(G4String file);
    static STbeamReadin* mInstancePtr;
    
};


#endif 