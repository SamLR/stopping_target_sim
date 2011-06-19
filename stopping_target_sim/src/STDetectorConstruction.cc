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

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"

STDetectorConstruction::STDetectorConstruction()
:  experimentalHall_log(0), tracker_log(0),
calorimeterBlock_log(0), calorimeterLayer_log(0),
experimentalHall_phys(0), calorimeterLayer_phys(0),
calorimeterBlock_phys(0), tracker_phys(0)
{;}

STDetectorConstruction::~STDetectorConstruction()
{
}


G4VPhysicalVolume* ConstructBox(G4Material *,G4RotationMatrix *, 
                                const G4ThreeVector &, const G4String &,
                                G4LogicalVolume*, G4LogicalVolume*, 
                                G4double, G4double, G4double,
                                G4bool, G4int, G4VSensitiveDetector*);


G4VPhysicalVolume* STDetectorConstruction::Construct()
{
    
    //------------------------------------------------------ materials
    
    G4double a;  // atomic mass
    G4double z;  // atomic number
    G4double density;
    G4int    nel; // number of elements (for mixtures)
    
    G4Material* Cu =
    new G4Material("Copper", z=29, a=63.54, density=8.94*g/mole);
    
    //Air (copied from ExN02DectectorConstruction
    G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
    
    G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
    Air->AddElement(N, 70*perCent);
    Air->AddElement(O, 30*perCent); 
    //------------------------------------------------------ volumes
    
    //------------------------------ experimental hall (world volume)
    //------------------------------ beam line along x axis
    
    G4double expHall_x = 3.0*m;
    G4double expHall_y = 1.0*m;
    G4double expHall_z = 1.0*m;
    G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
    experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                               Air,"expHall_log",0,0,0);
    experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                              experimentalHall_log,"expHall",0,false,0);
    
    //------------------------------ a copper bar (stopping target)
    G4double stX = 40*cm;
    G4double stY = 5*cm;
    G4double stZ = 5*mm;
    G4ThreeVector stPos = G4ThreeVector(0,0,0);
    cuStoppingTarget_phys = ConstructBox(Cu, 0, &stPos, // mat, rotation, translation
                                         "stopping_target", // name
                                         cuStoppingTarget_log, // current logical
                                         experimentalHall_log, // mother logical
                                         stX, stY, stZ); // box dimensions
    
    //------------------------------ a calorimeter block
    
    G4double block_x = 1.0*m;
    G4double block_y = 50.0*cm;
    G4double block_z = 50.0*cm;
    G4Box* calorimeterBlock_box = new G4Box("calBlock_box",block_x,
                                            block_y,block_z);
    calorimeterBlock_log = new G4LogicalVolume(calorimeterBlock_box,
                                               Pb,"caloBlock_log",0,0,0);
    G4double blockPos_x = 1.0*m;
    G4double blockPos_y = 0.0*m;
    G4double blockPos_z = 0.0*m;
    calorimeterBlock_phys = new G4PVPlacement(0,
                                              G4ThreeVector(blockPos_x,blockPos_y,blockPos_z),
                                              calorimeterBlock_log,"caloBlock",experimentalHall_log,false,0);
    
    //------------------------------ calorimeter layers
    
    G4double calo_x = 1.*cm;
    G4double calo_y = 40.*cm;
    G4double calo_z = 40.*cm;
    G4Box* calorimeterLayer_box = new G4Box("caloLayer_box",
                                            calo_x,calo_y,calo_z);
    calorimeterLayer_log = new G4LogicalVolume(calorimeterLayer_box,
                                               Al,"caloLayer_log",0,0,0);
    for(G4int i=0;i<19;i++) // loop for 19 layers
    {
        G4double caloPos_x = (i-9)*10.*cm;
        G4double caloPos_y = 0.0*m;
        G4double caloPos_z = 0.0*m;
        calorimeterLayer_phys = new G4PVPlacement(0,
                                                  G4ThreeVector(caloPos_x,caloPos_y,caloPos_z),
                                                  calorimeterLayer_log,"caloLayer",calorimeterBlock_log,false,i);
    }
    
    //------------------------------------------------------------------
    
    return experimentalHall_phys;

}


G4VPhysicalVolume* ConstructBox(G4Material *pMat,
                                G4RotationMatrix *pRot, 
                                const G4ThreeVector &tlate,
                                const G4String &pName,
                                G4LogicalVolume *pCurrentLogical,
                                G4LogicalVolume *pMotherLogical, 
                                G4double pX, G4double pY, G4double pZ,
                                G4bool pMany=false, G4int pCopyNo=0,
                                G4VSensitiveDetector *pSDetector=0)
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
    G4VPhysicalVolume *physicalVol = new G4PVPlacement(pRot, tlate, pCurrentLogical, pvName, pMotherLogical, pMany, pCopyNo);
    
    return physicalVol;
    
}






















