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
// $Id: STDetectorConstruction.cc,v 1.9 2006/06/29 17:47:19 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//

#include "STDetectorConstruction.hh"

STDetectorConstruction::STDetectorConstruction()
:   experimentalHall_log(0), experimentalHall_phys(0), 
    cuStoppingTarget_log(0), cuStoppingTarget_phys(0),
    counterA_log(0),         counterB_log(0),
    counterA_phys(0),        counterB_phys(0),
    counterA_sd(0),          counterB_sd(0)
{;}

STDetectorConstruction::~STDetectorConstruction() {;}


G4VPhysicalVolume* ConstructBox(G4Material *pMat,
                                G4RotationMatrix *pRot, 
                                const G4ThreeVector &tlate,
                                const G4String &pName,
                                G4LogicalVolume *pCurrentLogical,
                                G4LogicalVolume *pMotherLogical, 
                                G4double pX, G4double pY, G4double pZ,
                                G4bool pMany=false, G4int pCopyNo=0,
                                G4VSensitiveDetector *pSDetector=0);


G4VPhysicalVolume* STDetectorConstruction::Construct()
{
    
    //------------------------------------------------------------------------------ 
    // Define Materials
    //------------------------------------------------------------------------------    
    
    G4double a;  // atomic mass
    G4double z;  // atomic number
    G4double density;
    G4int    nel; // number of elements (for mixtures)
    
    G4Material* Cu =
    new G4Material("Copper", z=29, a=63.54*g/mole, density=8.94*g/cm3);
    
    //Air (copied from ExN02DectectorConstruction)
    G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
    
    G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
    Air->AddElement(N, 70*perCent);
    Air->AddElement(O, 30*perCent); 
    
    
    //------------------------------------------------------------------------------
    // Define Volumes
    //------------------------------------------------------------------------------
    
    // +++++++++
    // Experimental hall, beam is assumed to be along the x axis
    
    G4double expHall_x = 1.0*m;
    G4double expHall_y = 1.0*m;
    G4double expHall_z = 1.0*m;
    
    G4Box* experimentalHall_box = new G4Box("expHall_box",
                                            expHall_x,expHall_y,expHall_z);
    
    experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                               Air,"expHall_log",0,0,0);

    experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                              experimentalHall_log,"expHall",0,false,0);
    
    // +++++++++
    // Stopping target (copper)
    G4double st_x =  6*mm; // stopping target dimensions
    G4double st_y = 37*cm;
    G4double st_z =  8*cm;
    
    G4ThreeVector stPos = G4ThreeVector(0,0,0);
    
    cuStoppingTarget_phys = ConstructBox(Cu, 0, stPos, "stopping_target",
                                         cuStoppingTarget_log,
                                         experimentalHall_log, 
                                         st_x, st_y, st_z);

    // +++++++++
    // Counters (A&B currently virtual detectors)
    
    G4SDManager* sdMan = G4SDManager::GetSDMpointer(); // create the detector manager
    
    G4double c_x = 3.5*mm; // counter dimensions
    G4double c_y = 40*cm;
    G4double c_z = 50*cm;
    
    G4ThreeVector counterA_pos = G4ThreeVector(6*mm, 0, 0);
    
    counterA_phys = ConstructBox(Air, 0, counterA_pos, "counterA", 
                                 counterA_log, experimentalHall_log,
                                 c_x, c_y, c_z, false, 0, counterA_sd);
    
    sdMan->AddNewDetector(counterA_sd); // add the counterA
    
    G4ThreeVector counterB_pos = G4ThreeVector(-6*mm, 0, 0);
    counterB_phys = ConstructBox(Air, 0, counterB_pos, "counterB", 
                                 counterB_log, experimentalHall_log,
                                 c_x, c_y, c_z, false, 0, counterB_sd); 
    
    sdMan->AddNewDetector(counterB_sd);
    
    //------------------------------------------------------------------------------
    // Create senstive detector manager
    //------------------------------------------------------------------------------
    
    
    
    return experimentalHall_phys;

}


G4VPhysicalVolume* ConstructBox(G4Material *pMat,
                                G4RotationMatrix *pRot, 
                                const G4ThreeVector &tlate,
                                const G4String &pName,
                                G4LogicalVolume *pCurrentLogical,
                                G4LogicalVolume *pMotherLogical, 
                                G4double pX, G4double pY, G4double pZ,
                                G4bool pMany, G4int pCopyNo,
                                G4VSensitiveDetector *pSDetector)
{
    
    // This will generate a G4Solid box, then generate an associated 
    // logical volume then attach all that to a physical volume and return it.
    G4String boxName = "_name";
    boxName.prepend(pName);
    G4Box *box = new G4Box(boxName, pX, pY, pZ);
    
    if (pCurrentLogical == 0)
    { // If the logical volume doesn't exist: make it.
        G4String lvName = "_LV";
        lvName.prepend(pName);
        pCurrentLogical = new G4LogicalVolume(box, pMat, lvName, 0,pSDetector, 0, true);
    } 
    
    G4String pvName = "_PV";
    pvName.prepend(pName);
    G4VPhysicalVolume* physicalVol = new G4PVPlacement(pRot, tlate, pCurrentLogical, pvName, pMotherLogical, pMany, pCopyNo);
    
    return physicalVol;
    
}






















