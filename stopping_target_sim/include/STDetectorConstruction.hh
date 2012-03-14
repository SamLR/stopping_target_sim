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
class G4VSensitiveDetector;
class G4MagneticField;

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"


class STDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    STDetectorConstruction();
    ~STDetectorConstruction();

    G4VPhysicalVolume* Construct();

  private:
    void defineMaterials();
    void surfaceProperties();
    // TODO: add hitcollection
    
    // The magnetic field
    G4MagneticField* magField;
    
    // Logical volumes
    //

    G4LogicalVolume* expHall_log;
    G4LogicalVolume* container_log;
    G4LogicalVolume* cuStoppingTarget_log;
    G4LogicalVolume* counterA_log;
    G4LogicalVolume* counterB_log;
    G4LogicalVolume* wrapA_log;
    G4LogicalVolume* wrapB_log;
    G4LogicalVolume* mppcA1_log;
    G4LogicalVolume* mppcA2_log;
    G4LogicalVolume* mppcB1_log;
    G4LogicalVolume* mppcB2_log;
    // 0:x- 1:x+ 2:y- 3:y+ 4:z- 5:z+
    G4LogicalVolume* mon_box_log[6];
    
    // Physical volumes
    G4VPhysicalVolume* expHall_phys;
    G4VPhysicalVolume* container_phys;
    G4VPhysicalVolume* cuStoppingTarget_phys;
    G4VPhysicalVolume* counterA_phys;
    G4VPhysicalVolume* counterB_phys;
    G4VPhysicalVolume* wrapA_phys;
    G4VPhysicalVolume* wrapB_phys;
    G4VPhysicalVolume* mppcA1_phys;
    G4VPhysicalVolume* mppcA2_phys;
    G4VPhysicalVolume* mppcB1_phys;
    G4VPhysicalVolume* mppcB2_phys;
    // 0:x- 1:x+ 2:y- 3:y+ 4:z- 5:z+
    G4VPhysicalVolume* mon_box_phys[6];

    
    // Sensitive Detectors
    G4VSensitiveDetector* mppcA1_sd;
    G4VSensitiveDetector* mppcA2_sd;
    G4VSensitiveDetector* mppcB1_sd;
    G4VSensitiveDetector* mppcB2_sd;
    
    G4VSensitiveDetector* box_sd[6];
    G4VSensitiveDetector* monA_sd;
    G4VSensitiveDetector* monB_sd;
    
};

#endif

