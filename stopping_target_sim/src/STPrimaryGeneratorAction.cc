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

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4RandomDirection.hh"

STPrimaryGeneratorAction::STPrimaryGeneratorAction()
// TODO look at getting data for detector from that class
{
    // this is vaguely close to the energy of an electron produced in mu decay
    G4double electron_energy = 80.0*MeV;
    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("e-");
    
    particleGun->SetParticleDefinition(particle);
    
    particleGun->SetParticleEnergy(electron_energy);
}

STPrimaryGeneratorAction::~STPrimaryGeneratorAction()
{
    delete particleGun;
}

void STPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
    // Cu target is 6 x 370 x 80 mm; randomly position the e- with this volume
    G4double pos_x =   6 * (G4UniformRand() - 0.5) * mm;
    G4double pos_y = 370 * (G4UniformRand() - 0.5) * mm;
    G4double pos_z =  80 * (G4UniformRand() - 0.5) * mm;
    
    particleGun->SetParticlePosition(G4ThreeVector(pos_x, pos_y, pos_z));
    // use G4RandomDirection to ensure uniform dist across 4pi solid angle
    particleGun->SetParticleMomentumDirection(G4RandomDirection());
    
    particleGun->GeneratePrimaryVertex(anEvent);
}


