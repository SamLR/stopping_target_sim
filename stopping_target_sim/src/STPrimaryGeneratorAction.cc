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

STPrimaryGeneratorAction::STPrimaryGeneratorAction()
{
    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e-"));
    particleGun->SetParticleEnergy(50*MeV);
    particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
}

STPrimaryGeneratorAction::~STPrimaryGeneratorAction()
{
    delete particleGun;
}

void STPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // generate a random location somewhere within the copper bar
    // TODO: fewer magic numbers
    G4double x_pos =   3*mm * (G4UniformRand() - 0.5);
    G4double y_pos = 185*mm * (G4UniformRand() - 0.5);
    G4double z_pos =  40*mm * (G4UniformRand() - 0.5);
    
    G4ThreeVector position = G4ThreeVector(x_pos, y_pos, z_pos);
    particleGun->SetParticlePosition(position);
    
    
    G4ThreeVector momentum_dir = G4ThreeVector();// make this random in 4pi
    particleGun->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0));
    particleGun->GeneratePrimaryVertex(anEvent);
}


