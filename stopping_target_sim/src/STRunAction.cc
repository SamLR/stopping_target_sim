/*
 *  STRunAction.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 26/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */


#include "STRunAction.hh"
#include "STanalysis.hh"
#include "G4Run.hh"

STRunAction::STRunAction(){;}
STRunAction::~STRunAction()
{
//    mAnalysis->close();
}

void STRunAction::BeginOfRunAction(const G4Run* aRun)
{
//    mAnalysis = new STanalysis("out.root");
}

void STRunAction::EndOfRunAction(const G4Run* aRun)
{
//    G4cout << "updating analysis file" << G4endl;
//    mAnalysis->update();
}