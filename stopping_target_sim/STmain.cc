//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN01.cc,v 1.6 2006/06/29 17:47:10 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - exampleN01
// --------------------------------------------------------------

// Mandatory includes for geant
#include "G4RunManager.hh"
#include "G4UImanager.hh"

// include for visualisation 
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

// include for interactive mode
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// includes for physics etc
#include "STDetectorConstruction.hh"
#include "STPhysicsList.hh"
#include "STPrimaryGeneratorAction.hh"

int main(int argc, char** argv)
{
    // Construct the default run manager
    G4RunManager* runManager = new G4RunManager;
    
    // initialise detector & physics
    runManager->SetUserInitialization(new STDetectorConstruction);
    runManager->SetUserInitialization(new STPhysicsList);
    
#ifdef G4VIS_USE // set up visualisation manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif 
    
    // initialise primary action
    runManager->SetUserAction(new STPrimaryGeneratorAction);
    
    // Initialize G4 kernel
    //
    runManager->Initialize();
    
    // Get the pointer to the UI manager and set verbosities
    //
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 1");
    UImanager->ApplyCommand("/event/verbose 1");
    UImanager->ApplyCommand("/tracking/verbose 1");
    
    if(argc==1){
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
        ui->SessionStart();
        delete ui;
#endif
    }
    else{
        G4String command = "/control/execute ";
        G4String filename = argv[1];
        UImanager->ApplyCommand(command+filename);
    }
    
    // Job termination
    //
    // Free the store: user actions, physics_list and detector_description are
    //                 owned and deleted by the run manager, so they should not
    //                 be deleted in the main() program !
    //
    delete runManager;
    
    return 0;
}


