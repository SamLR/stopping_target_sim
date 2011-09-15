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

#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"

// get special managers for muons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

// EM processes
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

// EM for electrons
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

// EM for muons
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCaptureAtRest.hh"

// EM for hadrons
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

#include "G4Decay.hh"

// optical Photon processes     
#include "G4OpticalPhoton.hh"
#include "G4Cerenkov.hh" 

#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh" 

// for any ions that crop up
#include "G4ionIonisation.hh"

STPhysicsList::STPhysicsList(): G4VUserPhysicsList()
{;}

STPhysicsList::~STPhysicsList()
{;}

void STPhysicsList::ConstructParticle()
{
    ConstructBosons();
    ConstructLeptons();
    ConstructBaryons();
    ConstructMesons();
    ConstructIons();
    G4Gamma::GammaDefinition();
}

void STPhysicsList::ConstructProcess()
{
    ConstructEM();
    ConstructDecay();
    ConstructOp();
    AddTransportation();
}

void STPhysicsList::SetCuts()
{
    // uppress error messages even in case e/gamma/proton do not exist            
    G4int temp = GetVerboseLevel(); 
    SetVerboseLevel(0);                                                           
    //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
    //   the default cut value for all particle types 
    SetCutsWithDefault();   
    
    // Retrieve verbose level
    SetVerboseLevel(temp);  
}

void STPhysicsList::ConstructBosons()
{
    // pseudo-particles
    G4Geantino::GeantinoDefinition();
    G4ChargedGeantino::ChargedGeantinoDefinition();
    
    // gamma
    G4Gamma::GammaDefinition();
    
    // optical photon
    G4OpticalPhoton::OpticalPhotonDefinition(); 
}

void STPhysicsList::ConstructLeptons()
{ 
    // leptons
    //  e+/-
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
    // mu+/-
    G4MuonPlus::MuonPlusDefinition();
    G4MuonMinus::MuonMinusDefinition();
    // nu_e
    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();
    // nu_mu
    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void STPhysicsList::ConstructMesons()
{
    G4PionPlus::PionPlusDefinition();
    G4PionMinus::PionMinusDefinition();
    G4PionZero::PionZeroDefinition();
}

void STPhysicsList::ConstructBaryons()
{
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();
    
    G4Neutron::NeutronDefinition();
    G4AntiNeutron::AntiNeutronDefinition();
}

void STPhysicsList::ConstructIons()
{    
    G4Deuteron::DeuteronDefinition();
    G4Triton::TritonDefinition();
    G4Alpha::AlphaDefinition();
    G4GenericIon::GenericIonDefinition();
}

void STPhysicsList::ConstructEM()
{
    theParticleIterator->reset();
    
    while( (*theParticleIterator)() )
    {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        
        if (particleName == "gamma") 
        {
            // gamma         
            pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
            pmanager->AddDiscreteProcess(new G4ComptonScattering);
            pmanager->AddDiscreteProcess(new G4GammaConversion);
            
        } else if (particleName == "e-") 
        {
            //electron
            pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);      
            
        } else if (particleName == "e+") 
        {
            //positron
            pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
            pmanager->AddProcess(new G4eplusAnnihilation,    0,-1, 4);
            
        } else if (particleName == "mu+" ) 
        {
            pmanager = G4MuonPlus::MuonPlus()->GetProcessManager();
            //muon  
            pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4MuIonisation,         -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 3);
            pmanager->AddProcess(new G4MuPairProduction,     -1, 4, 4);   
            
        } else if (particleName == "mu-" ) 
        {
            pmanager = G4MuonMinus::MuonMinus()->GetProcessManager();
            //muon  
            pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4MuIonisation,         -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 3);
            pmanager->AddProcess(new G4MuPairProduction,     -1, 4, 4);   
            
            pmanager->AddRestProcess(new G4MuonMinusCaptureAtRest);
            
        } else if( particleName == "proton" ||
                  particleName == "pi-"     ||
                  particleName == "pi+"     ) 
        {
            //proton  
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
            pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
            pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);       
            
        } else if( particleName == "alpha" ||
                  particleName == "He3" )     
        {
            //alpha 
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);
            
        } else if( particleName == "GenericIon" ) 
        { 
            //Ions 
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);
            
        } else if ((!particle->IsShortLived())                       &&
                   (particle->GetPDGCharge()    != 0.0)              && 
                   (particle->GetParticleName() != "chargedgeantino")) 
        {
            //all others charged particles except geantino
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
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


void STPhysicsList::ConstructOp()
{
    theCerenkovProcess           = new G4Cerenkov("Cerenkov");
    theScintillationProcess      = new G4Scintillation("Scintillation");
    theAbsorptionProcess         = new G4OpAbsorption();
    theRayleighScatteringProcess = new G4OpRayleigh();
    theMieHGScatteringProcess    = new G4OpMieHG();
    theBoundaryProcess           = new G4OpBoundaryProcess();
    
    theCerenkovProcess->SetMaxNumPhotonsPerStep(20);
    theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
    theCerenkovProcess->SetTrackSecondariesFirst(true);
    
    theScintillationProcess->SetScintillationYieldFactor(1.);
    theScintillationProcess->SetTrackSecondariesFirst(true);
    
    // Use Birks Correction in the Scintillation process
//    
//    G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
//    theScintillationProcess->AddSaturation(emSaturation);
    
    G4OpticalSurfaceModel themodel = unified;
    theBoundaryProcess->SetModel(themodel);
    
    theParticleIterator->reset();
    while( (*theParticleIterator)() )
    {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        
        if (theCerenkovProcess->IsApplicable(*particle)) {
            
            pmanager->AddProcess(theCerenkovProcess);
            pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
        }
        
        if (theScintillationProcess->IsApplicable(*particle)) 
        {
            pmanager->AddProcess(theScintillationProcess);
            pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
            pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
        }
        
        if (particleName == "opticalphoton") 
        {
            G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
            pmanager->AddDiscreteProcess(theAbsorptionProcess);
            pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
            pmanager->AddDiscreteProcess(theMieHGScatteringProcess);
            pmanager->AddDiscreteProcess(theBoundaryProcess);
        }
    }
}


