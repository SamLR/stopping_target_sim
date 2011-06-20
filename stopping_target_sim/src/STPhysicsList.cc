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
// $Id: STPhysicsList.cc,v 1.6 2006/06/29 17:47:21 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
// 

#include "STPhysicsList.hh"


STPhysicsList::STPhysicsList()
{;}

STPhysicsList::~STPhysicsList()
{;}

void STPhysicsList::ConstructParticle()
{
    // In this method, static member functions should be called
    // for all particles which you want to use.
    // This ensures that objects of these particle types will be
    // created in the program. 
    G4BosonConstructor pBosonConstructor;
    pBosonConstructor.ConstructParticle();
    
    G4LeptonConstructor pLeptonConstructor;
    pLeptonConstructor.ConstructParticle();
}

void STPhysicsList::ConstructProcess()
{
    // Define transportation process
    
    AddTransportation();
    ConstructEM();
    ConstructDecay();
}

void STPhysicsList::ConstructEM()
{
    theParticleIterator->reset();
    
    while ( (*theParticleIterator)() ) 
    {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        
        if (particleName == "gamma")
        {
            // photon
            pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
            pmanager->AddDiscreteProcess(new G4ComptonScattering);
            pmanager->AddDiscreteProcess(new G4GammaConversion);
            
        } else if (particleName == "e-") {
            //electron
            pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
            pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);      
            
        } else if (particleName == "e+") {
            //positron
            pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
            pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);
            pmanager->AddProcess(new G4eplusAnnihilation,   0,-1, 4);
            
        } else if( particleName == "mu+" || 
                  particleName == "mu-"    ) {
            //muon  
            pmanager->AddProcess(new G4MuMultipleScattering,-1, 1, 1);
            pmanager->AddProcess(new G4MuIonisation,       -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung,   -1, 3, 3);
            pmanager->AddProcess(new G4MuPairProduction,   -1, 4, 4);
            
        }
    }
}

void STPhysicsList::ConstructDecay()
{
    // Add Decay Process
    G4Decay* theDecayProcess = new G4Decay();
    theParticleIterator->reset();
    while( (*theParticleIterator)() )
    {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        
        if (theDecayProcess->IsApplicable(*particle)) 
        { 
            pmanager ->AddProcess(theDecayProcess);
            // set ordering for PostStepDoIt and AtRestDoIt
            pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
            pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
        }
    }
}



void STPhysicsList::SetCuts()
{
    // uppress error messages even in case e/gamma/proton do not exist            
    G4int temp = GetVerboseLevel();                                                SetVerboseLevel(0);                                                           
    //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
    //   the default cut value for all particle types 
    SetCutsWithDefault();   
    
    // Retrieve verbose level
    SetVerboseLevel(temp);  
}

