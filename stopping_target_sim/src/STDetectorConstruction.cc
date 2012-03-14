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


#include "STcounterSD.hh"
#include "STMonitorSD.hh"
#include "STDetectorConstruction.hh"
#include "STTabulatedField3D.hh"

// managers
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"

// geometry stuff
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"

// optical properties
#include "G4OpticalSurface.hh"   
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"   


STDetectorConstruction::STDetectorConstruction()
{
    magField = new STTabulatedField3D();
}

STDetectorConstruction::~STDetectorConstruction() {;}


G4VPhysicalVolume* STDetectorConstruction::Construct()
{
    defineMaterials();
    
    G4Material* Air   = G4Material::GetMaterial("Air");
    G4Material* Al    = G4Material::GetMaterial("Al");
    G4Material* Cu    = G4Material::GetMaterial("Cu");
    G4Material* Scint = G4Material::GetMaterial("Pethylene");
    
    // Detector layout
    // travelling in the z (beam) direction
    // { world vol -- needed by G4
    //  {container -- used to manipulate the detector as a whole
    //      { Monitor 'box' -- slabs creating an enclosing monitor
    //          { monitor -- monitor to represent the scintillator}
    //          { Al wrapper -- outside of scintillator 
    //              { scintillator
    //                  { MPPCs (x2) -- cube of air in the scintillator }
    //              }
    //          }
    //          Air gap -- represents the Al supports - not modelled
    //          { stopping target -- Copper}
    // ... 
    // the rest is the inverse
    
    
    //--------------------------------------------------------------------------
    // Definition of directions
    //--------------------------------------------------------------------------
    // x = perpendicular to y & z
    // y = vertically (positive = up)
    // z = beam direction (positive = direction of particles)
    //--------------------------------------------------------------------------
    // NB: All measurements are divided by 2 at point of use (due to G4's odd
    //     extrusion from centre policy)
    
    G4double expHall_x = 1.0*m; // Experimental hall 
    G4double expHall_y = 1.0*m;
    G4double expHall_z = 1.0*m;
    
    G4double st_x = 37.0*cm; // stopping target dimensions
    G4double st_y =  8.0*cm; 
    G4double st_z =  0.6*cm;  
    
    G4double c_x = 40.0*cm; // counter dimensions
    G4double c_y =  5.0*cm;
    G4double c_z =  0.35*cm; 
    
    G4double al_thickness = 0.3*cm; // Aluminium supports (not modelled)
    
    G4double wrap = 0.3*cm; // excess width due to Al foil (gives 1.5mm width)

    G4double mppc_d = 0.1*cm; // model MPPC as a 1x1x1mm cube of Air
    
    G4double theta = -36.0*deg; // angle by which the ST is rotated about Y

    
    G4double beampipe_offset = 5.0*cm; // distance from end of beampipe to target
        
    //--------------------------------------------------------------------------
    // Define Volumes
    //--------------------------------------------------------------------------
    
    // +++++++++
    // Experimental hall, beam is assumed to be along the z axis
    
    G4Box* expHall_box = new G4Box("expHall_box",
                                   expHall_x/2 ,expHall_y/2 ,expHall_z/2);
    
    expHall_log = new G4LogicalVolume(expHall_box,
                                      Air,"expHall_log",0,0,0);
    
    expHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                     expHall_log,"expHall_phys",
                                     0,false,0);
    
    // +++++++++
    // Container region (air) 
    // This allows all the other regions to be manipulated as a single object
    
    // Derived values
    G4double contX = c_x + wrap + 2*c_z; // width of counter and 2 monitors    
    G4double contY = st_y + 2*c_z; // height of the target and 2 monitors
    // 2 Al supports, 4 monitors, 2 counters , 2 Al wrappers (monitors_z == c_z)
    G4double contZ = 2*al_thickness + 4*c_z + 2*wrap + st_z; 

    G4ThreeVector container_offset = G4ThreeVector(0,0,beampipe_offset);
    G4RotationMatrix* rotation = new G4RotationMatrix();
    rotation->rotateY(theta);

    
    G4Box* container_box = new G4Box("container", contX/2, contY/2, contZ/2);
    container_log  = new G4LogicalVolume(container_box, Air, "containter_log");
    container_phys = new G4PVPlacement(rotation, container_offset, container_log, 
                                       "container_phys", expHall_log, false, 0);
    
    // The detector itself, working from the centre outwards
    
    // +++++++++
    // Stopping target (copper)
    // 
    G4ThreeVector origin = G4ThreeVector(0, 0, 0);
    G4Box* target_box = new G4Box("target", st_x/2, st_y/2, st_z/2);
    cuStoppingTarget_log = new G4LogicalVolume(target_box, Cu, "target_log");
    cuStoppingTarget_phys = new G4PVPlacement(0, origin, cuStoppingTarget_log, 
                                              "target_phys", container_log, 
                                              false, 0);
    // +++++++++
    // Counters     
    // The solids 
    G4Box* counter_box = new G4Box("counter", c_x/2, c_y/2, c_z/2);
    G4Box* counter_wrap = new G4Box("wrapper",
                                    (c_x + wrap)/2, (c_y + wrap)/2, (c_z + wrap)/2);

    // positional offset: width of Al frame + half widths of Cu & scint
    G4double counter_Zoffset = (st_z + c_z + wrap)/2 + al_thickness; 

    // location of counter & wrap A
    G4ThreeVector a_pos = G4ThreeVector(0, 0, counter_Zoffset);
    
    // Al wrapper for counter A;
    wrapA_log    = new G4LogicalVolume(counter_wrap, Al, "wrapA_log");
    wrapA_phys   = new G4PVPlacement(0, a_pos, wrapA_log, 
                                     "wrapA_phys", container_log, 
                                     false, 0);
    // Counter A
    counterA_log  = new G4LogicalVolume(counter_box, Scint, "counterA_log");
    counterA_phys = new G4PVPlacement(0, G4ThreeVector(), counterA_log, 
                                      "counterA_phys", wrapA_log, 
                                      false, 0);
    
    // location for counter & wrap B
    G4ThreeVector b_pos = G4ThreeVector(0, 0, -counter_Zoffset);
    
    // Al wrapper for counter B
    wrapB_log    = new G4LogicalVolume(counter_wrap, Al, "wrapB_log"); 
    wrapB_phys   = new G4PVPlacement(0, b_pos, wrapB_log, 
                                     "wrapb_phys", container_log, 
                                     false, 0);    
    // Counter B
    counterB_log  = new G4LogicalVolume(counter_box, Scint, "counterB_log");
    counterB_phys = new G4PVPlacement(0, G4ThreeVector(), counterB_log, 
                                      "counterB_phys", wrapB_log, 
                                      false, 0);
    
    // MPPCs
    // The solid
    G4Box* mppc = new G4Box("MPPC", mppc_d/2, mppc_d/2, mppc_d/2);
    
    G4ThreeVector mppc1 = G4ThreeVector( (c_x - mppc_d)/2, 0, 0);
    G4ThreeVector mppc2 = G4ThreeVector(-(c_x - mppc_d)/2, 0, 0);
    // try having the MPPCs inside the scint
    mppcA1_log           = new G4LogicalVolume(mppc, Scint, "mppcA1_log");
    mppcA1_phys          = new G4PVPlacement(0, mppc1, mppcA1_log, 
                                             "mppcA1_phys", counterA_log, 
                                             false, 0);
    mppcA2_log           = new G4LogicalVolume(mppc, Scint, "mppcA2_log");
    mppcA2_phys          = new G4PVPlacement(0, mppc2, mppcA2_log, 
                                             "mppcA2_phys", counterA_log, 
                                             false, 0);
    
    mppcB1_log           = new G4LogicalVolume(mppc, Scint, "mppcB1_log");    
    mppcB1_phys          = new G4PVPlacement(0, mppc1, mppcB1_log, 
                                             "mppcB1_phys", counterB_log, 
                                             false, 0);
    mppcB2_log           = new G4LogicalVolume(mppc, Scint, "mppcB2_log");
    mppcB2_phys          = new G4PVPlacement(0, mppc2, mppcB2_log, 
                                             "mppcB2_phys", counterB_log, 
                                             false, 0);  
    // ==========
    // Box monitors (enclose the entire detector)
    // <x,y,z>_max give the maximum dimensions of the monitor box
    G4double z_max = st_z + 4*c_z + 2*wrap + 2*al_thickness; // includes the 2 z-face monitors
    G4double y_max = st_y + 2*c_z;
    G4double x_max = 2*c_z + c_x + wrap; 
    
    // x-face is middle, hence abutted by z-face and abutts y-face
    G4Box* x_face_mon = new G4Box("x_face", c_z/2, st_y/2, z_max/2);
    // y-face is outer most hence abutted by x and z-face monitors (in z_thickness & param)
    G4Box* y_face_mon = new G4Box("y_face", x_max/2, c_z/2, z_max/2);
    // z-face is inner most, abutts both x and y faces
    G4Box* z_face_mon = new G4Box("z_face", (x_max - 2*c_z)/2, st_y/2, c_z/2);
    
    // positional offsets, these move the centre of the solid hence removal of 
    // a monitor's width from each maximum
    G4ThreeVector x_box_off = G4ThreeVector((x_max-c_z)/2, 0, 0);
    G4ThreeVector y_box_off = G4ThreeVector(0,(y_max - c_z)/2, 0);
    G4ThreeVector z_box_off = G4ThreeVector(0, 0, (z_max - c_z)/2);
    
    
    G4String face_names [] = {"x_neg", "x_pos", "y_neg", "y_pos", "z_neg", "z_pos"};
    G4ThreeVector face_pos [] = { -1*x_box_off, x_box_off,
                                  -1*y_box_off, y_box_off,
                                  -1*z_box_off, z_box_off};
    G4Box* boxes [] = {x_face_mon, x_face_mon, y_face_mon, 
                       y_face_mon, z_face_mon, z_face_mon};
    // array indexs
    // 0:x- 1:x+ 2:y- 3:y+ 4:z- 5:z+
    
    for (int i(0); i<6; ++i) 
    {
        mon_box_log[i]  = new G4LogicalVolume(boxes[i], Air, face_names[i]);
        mon_box_phys[i] = new G4PVPlacement(0, face_pos[i], mon_box_log[i], 
                                            face_names[i], container_log, false, 0); 
    }
    
    //--------------------------------------------------------------------------
    // Create senstive detector manager for the MPPCs
    //--------------------------------------------------------------------------
    // create the detector manager
    G4SDManager* sdMan = G4SDManager::GetSDMpointer(); 
    
    G4String fileroot="../../../output";
    G4String mppcFile = fileroot+"/mppc_out.root";
    G4String truthFile = fileroot+"/truth_out.root";
    G4String boxFile = fileroot+"/box_out.root";
    
    mppcA1_sd = new STcounterSD("mppcA1", mppcFile, "mppcA1");
    sdMan->AddNewDetector(mppcA1_sd); 
    mppcA1_log->SetSensitiveDetector(mppcA1_sd);    
    
    mppcA2_sd = new STcounterSD("mppcA2", mppcFile, "mppcA2");
    sdMan->AddNewDetector(mppcA2_sd); 
    mppcA2_log->SetSensitiveDetector(mppcA2_sd);    
    
    mppcB1_sd = new STcounterSD("mppcB1", mppcFile, "mppcB1");
    sdMan->AddNewDetector(mppcB1_sd); 
    mppcB1_log->SetSensitiveDetector(mppcB1_sd);    
    
    mppcB2_sd = new STcounterSD("mppcB2", mppcFile, "mppcB2");
    sdMan->AddNewDetector(mppcB2_sd); 
    mppcB2_log->SetSensitiveDetector(mppcB2_sd);   
    
    monA_sd= new STMonitorSD("monA", truthFile, "monA");
    sdMan->AddNewDetector(monA_sd); 
    counterA_log->SetSensitiveDetector(monA_sd);   
    
    monB_sd= new STMonitorSD("monB", truthFile, "monB");
    sdMan->AddNewDetector(monB_sd); 
    counterB_log->SetSensitiveDetector(monB_sd);      
    
    for (int i(0); i < 6; ++i) {
        box_sd[i] = new STMonitorSD(face_names[i], boxFile, face_names[i]);
        sdMan->AddNewDetector(box_sd[i]);
        mon_box_log[i]->SetSensitiveDetector(box_sd[i]);

    }
    
    //--------------------------------------------------------------------------   
    //  Magnetic Field 
    //--------------------------------------------------------------------------
    
    static G4bool fieldIsInitialized = false;
    if(!fieldIsInitialized)
    {
        G4FieldManager* pFieldMgr;
        
        G4MagneticField* magField = new STTabulatedField3D();
        
        G4TransportationManager* tMan = 
        G4TransportationManager::GetTransportationManager();
        
        pFieldMgr = tMan->GetFieldManager();
                
        G4ChordFinder *pChordFinder = new G4ChordFinder(magField);
        pFieldMgr->SetChordFinder( pChordFinder );
        
        pFieldMgr->SetDetectorField(magField);
        
        fieldIsInitialized = true;
    }  
    
    // finally set the surface properties
    surfaceProperties();
    
    // return the lot
    return expHall_phys;
}



void STDetectorConstruction::defineMaterials()
{
    
    
    //-------------------------------------------------------------------------- 
    // Define Materials
    //--------------------------------------------------------------------------    
    
    G4double a;  // atomic mass
    G4double z;  // atomic number
    G4double density;
    G4int    nel; // number of elements (for mixtures)
    
    // bulk materials
    G4Material* Cu =
    new G4Material("Cu", z=29, a=63.54*g/mole, density=8.94*g/cm3);
    
    G4Material* Al = 
    new G4Material("Al", z=13., a=26.98*g/mole, density=2.7*g/cm3);  
    
    //Air (copied from ExN02DectectorConstruction)
    G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
    
    G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
    Air->AddElement(N, 70*perCent);
    Air->AddElement(O, 30*perCent); 
    
    //Polyethylene (scintillator)
    G4Element* H = new G4Element("H", "H", z=1., a=1.01*g/mole);
    G4Element* C = new G4Element("C", "C", z=6., a=12.01*g/mole);
    
    G4Material* Pethylene = new G4Material("Pethylene", density=1.032*g/cm3, 2);
    Pethylene->AddElement(H, 2);
    Pethylene->AddElement(C, 4);
    
    //Polyethylene properties
    const G4int n_entries = 3;
    
    G4double Pethylene_Energy[n_entries] = { 2.52*eV , 2.92*eV, 3.22*eV};
    G4double Pethylene_SCINT[n_entries]  = { 0.1, 1.0, 0.1};
    G4double Pethylene_RIND[n_entries]   = { 1.58, 1.58, 1.58};
    G4double Pethylene_ABSL[n_entries]   = { 210.0*cm , 210.0*cm, 210.0*cm};
    
    G4MaterialPropertiesTable* Pethylene_mt = new G4MaterialPropertiesTable();
    Pethylene_mt->AddProperty("FASTCOMPONENT", 
                              Pethylene_Energy, Pethylene_SCINT, n_entries);
    Pethylene_mt->AddProperty("SLOWCOMPONENT", 
                              Pethylene_Energy, Pethylene_SCINT, n_entries);
    Pethylene_mt->AddProperty("RINDEX",        
                              Pethylene_Energy, Pethylene_RIND,  n_entries);
    Pethylene_mt->AddProperty("ABSLENGTH",     
                              Pethylene_Energy, Pethylene_ABSL,  n_entries);
    Pethylene_mt->AddConstProperty("SCINTILLATIONYIELD",10000.0/MeV); 
    Pethylene_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
    Pethylene_mt->AddConstProperty("FASTTIMECONSTANT",3.80*ns);
    Pethylene_mt->AddConstProperty("SLOWTIMECONSTANT",14.2*ns);
    Pethylene_mt->AddConstProperty("YIELDRATIO",1.0);
    Pethylene->SetMaterialPropertiesTable(Pethylene_mt);   
    
}

void STDetectorConstruction::surfaceProperties()
{    
    const G4int num = 3;
    G4double Ephoton[num] = {2.52*eV , 2.92*eV, 3.22*eV};
    G4double refl = 0.88; // according to reading this seems a reasonable value
    //**Scintillator housing properties
    G4double Reflectivity[num] = {refl, refl, refl};
    G4double Efficiency[num] = {0.0, 0.0, 0.0}; 
    
    G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable(); 
    scintHsngPT->AddProperty("REFLECTIVITY", Ephoton, Reflectivity, num);
    scintHsngPT->AddProperty("EFFICIENCY", Ephoton, Efficiency, num);
    
    G4OpticalSurface* OpScintHousingSurface =
        new G4OpticalSurface("wrapSurface",unified,groundbackpainted,dielectric_metal);
    
    OpScintHousingSurface->SetMaterialPropertiesTable(scintHsngPT);
    
    //**Create logical skin surfaces
    new G4LogicalSkinSurface("wrapB_surf",wrapA_log,
                             OpScintHousingSurface);
    new G4LogicalSkinSurface("wrapA_surf",wrapB_log,
                             OpScintHousingSurface);
    
} 














