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
:   expHall_log(0), expHall_phys(0), 
    cuStoppingTarget_log(0), cuStoppingTarget_phys(0),
    counterA_log(0),         counterB_log(0),
    counterA_phys(0),        counterB_phys(0),
    counterA_sd(0),          counterB_sd(0)
{;}

STDetectorConstruction::~STDetectorConstruction() {;}


G4VPhysicalVolume* STDetectorConstruction::Construct()
{
    G4double al_thickness = 3*mm;
    
    G4double expHall_x = 1.0*m;
    G4double expHall_y = 1.0*m;
    G4double expHall_z = 1.0*m;
    
    G4double st_x =  6*mm; // stopping target dimensions 
    G4double st_y = 37*cm;
    G4double st_z =  8*cm;
    
    G4double c_x =  3.5*mm; // counter dimensions 
    G4double c_y = 40.0*cm;
    G4double c_z =  5.0*cm;
    //-------------------------------------------------------------------------- 
    // Define Materials
    //--------------------------------------------------------------------------    
    
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
    
    
    //--------------------------------------------------------------------------
    // Define Volumes
    //--------------------------------------------------------------------------
    
    // +++++++++
    // Experimental hall, beam is assumed to be along the x axis
    
    G4Box* expHall_box = new G4Box("expHall_box",
                                            expHall_x,expHall_y,expHall_z);
    
    expHall_log = new G4LogicalVolume(expHall_box,
                                               Air,"expHall_log",0,0,0);

    expHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                              expHall_log,"expHall",
                                              0,false,0);
    // +++++++++
    // Stopping target (copper)
    G4ThreeVector stPos = G4ThreeVector(0,0,0);
    
    G4Box* target_phys = new G4Box("target", st_x, st_y, st_z);
    cuStoppingTarget_log = new G4LogicalVolume(target_phys, Cu, "target_log");
    cuStoppingTarget_phys = new G4PVPlacement(0, stPos, cuStoppingTarget_log, 
                                              "target_phys", expHall_log, 
                                              false, 0);
    // +++++++++
    // Counters (A&B currently virtual detectors)
    // positional offset: width of Al frame + half widths of Cu & scint
    G4double x_offset = st_x + c_x + al_thickness; 
    
    G4Box* counter = new G4Box("counter", c_x, c_y, c_z);
    
    // Counter A
    G4ThreeVector counterA_pos = G4ThreeVector(x_offset, 0, 0);
    counterA_log = new G4LogicalVolume(counter, Air, "counterA_log");
    counterA_phys = new G4PVPlacement(0, counterA_pos, counterA_log, 
                                      "counterA_phys", expHall_log, 
                                      false, 0);
    // Counter B
    G4ThreeVector counterB_pos = G4ThreeVector(-x_offset, 0, 0);
    counterB_log = new G4LogicalVolume(counter, Air, "counterB_log");
    counterB_phys = new G4PVPlacement(0, counterB_pos, counterB_log, 
                                      "counterB_phys", expHall_log, 
                                      false, 0);
    
    
    //--------------------------------------------------------------------------
    // Create senstive detector manager
    //--------------------------------------------------------------------------
    // create the detector manager
    G4SDManager* sdMan = G4SDManager::GetSDMpointer(); 
    
    
    counterA_sd = new STcounterSD("counterA");
    sdMan->AddNewDetector(counterA_sd); // add the counterA
    counterA_log->SetSensitiveDetector(counterA_sd);
    
    counterB_sd = new STcounterSD("counterB");
    sdMan->AddNewDetector(counterB_sd);
    counterB_log->SetSensitiveDetector(counterB_sd);
    
    return expHall_phys;

}






















