/*
 *  STRunAction.hh
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 26/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#ifndef STRUNACTION_HH
#define STRUNACTION_HH 1

#include "STRunAction.hh"

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class STRunAction : public G4UserRunAction
{
public:
    STRunAction();
    ~STRunAction();
    
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
};


#endif