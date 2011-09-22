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
// $Id: STPrimaryGeneratorAction.cc,v 1.6 2006/06/29 17:47:23 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//

#include "STPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"

#include "Randomize.hh"
#include "G4RandomDirection.hh"

#include "G4UImanager.hh"

#include "STanalysis.hh"

STPrimaryGeneratorAction::STPrimaryGeneratorAction(): 
    mFileMode(false), mBeamData(0), mParticleGun(0)
//    mFileMode(true), mBeamData(0), mParticleGun(0)
// add messenger to toggle file input & mono-energetic muon 
{
    if (mFileMode)
    {    
//        G4String file_path = "/Users/scook/code/MuSIC/MuSIC_simulation/stopping_target_sim/test_particles.txt";
        G4String file_path 
            = "/Users/scook/code/MuSIC/MuSIC_simulation/MuSIC_g4beamline/monitor6_9999990_initial_protons.txt";
        mBeamData =  STbeamReadin::getPointer(file_path);
        mParticleGun = new G4ParticleGun(1);
    } else 
    {
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        mParticleGun = new G4ParticleGun(1);
//        G4ParticleDefinition* particle = particleTable->FindParticle("OpticalPhoton"); 
        G4ParticleDefinition* particle = particleTable->FindParticle("mu-"); 
        mParticleGun->SetParticleDefinition(particle);
//        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
//        mParticleGun->SetParticleEnergy(0.002*keV);
        mParticleGun->SetParticleEnergy(5*MeV);
    }
}

STPrimaryGeneratorAction::~STPrimaryGeneratorAction()
{
    if (mBeamData) mBeamData->close();
    if (mParticleGun) delete mParticleGun;
}

void STPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
    if (mFileMode) 
    {
        inputParticle current;
        current = mBeamData->next();
        if (current.status < 0) 
        {
            // error, stop the current run
            STanalysis* analysis = STanalysis::getInitdPointer();
            analysis->close(TRUE);
            G4cout <<"Error: out of primaries, aborting run" << G4endl;
            G4UImanager* ui = G4UImanager::GetUIpointer();
            ui->ApplyCommand("/run/abort");
        }
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* particle =
                    particleTable->FindParticle(current.PDG_id);
        
        mParticleGun->SetParticleDefinition(particle);
        mParticleGun->SetParticlePosition(current.position);
        // position will need to be adjusted WRT to new geometry
        mParticleGun->SetParticleMomentum(current.momentum);
    } else 
    {
//        mParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 9.25*mm));
        mParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    }
    mParticleGun->GeneratePrimaryVertex(anEvent);

}



