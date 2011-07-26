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
// $Id: STPrimaryGeneratorAction.hh,v 1.5 2006/06/29 17:47:17 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//

#ifndef STPrimaryGeneratorAction_h
#define STPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

#include "STbeamReadin.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class STPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    STPrimaryGeneratorAction();
    ~STPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* mParticleGun;
    STbeamReadin* mBeamData;
    G4bool mFileMode;
};

#endif


