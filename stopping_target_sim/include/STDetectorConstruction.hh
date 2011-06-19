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
// $Id: STDetectorConstruction.hh,v 1.6 2006/06/29 17:47:13 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//

#ifndef STDetectorConstruction_H
#define STDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"

class STDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    STDetectorConstruction();
    ~STDetectorConstruction();

    G4VPhysicalVolume* Construct();
    void ConstructMaterials();

  private:
    
    // Logical volumes
    //
    G4LogicalVolume* cuStoppingTarget_log;
    G4LogicalVolume* experimentalHall_log;
    G4LogicalVolume* tracker_log;
    G4LogicalVolume* calorimeterBlock_log;
    G4LogicalVolume* calorimeterLayer_log;

    // Physical volumes
    //
    G4VPhysicalVolume* cuStoppingTarget_phys;
    G4VPhysicalVolume* experimentalHall_phys;
    G4VPhysicalVolume* calorimeterLayer_phys;
    G4VPhysicalVolume* calorimeterBlock_phys;
    G4VPhysicalVolume* tracker_phys;
};


G4VPhysicalVolume* ConstructBox(G4Material *pMat,
                                G4RotationMatrix *pRot, 
                                const G4ThreeVector &tlate,
                                const G4String &pName,
                                G4LogicalVolume *pCurrentLogical,
                                G4LogicalVolume *pMotherLogical, 
                                G4double pX, G4double pY, G4double pZ,
                                G4bool pMany=false, G4int pCopyNo=0,
                                G4VSensitiveDetector *pSDetector=0);

#endif

