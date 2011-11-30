/*
 *  STTabulatedField3D.hh
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 21/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#ifndef STTABULATEDFIELD3D_HH
#define STTABULATEDFIELD3D_HH

#include "globals.hh"
#include "G4MagneticField.hh"

#include <vector>

class STTabulatedField3DMessenger;

using namespace std;

class STTabulatedField3D: public G4MagneticField
{
private:
    // The actual GetField function; others are wrappers
    void GetField(FILE* file);
    // Clears the <x,y,z>Field vector3d and sets derived values 
    // (min, max, d, n and invert) to default values
    void ClearField();
    // Initialise the field: read in the file, set derived values etc
    void Init();
    
    // non-derived data
    // offsets of the bfield from the detector geometry
    double xOffset, yOffset, zOffset;
    // file to be read in
    char filename_m[256];
    // sets level on information to output
    int verbose;
    // flag for when defaults are changed
    bool defaultFlag;
    // messenger 
    STTabulatedField3DMessenger* messenger_m;
    
    // derived data
    // vector with 3 subscripts 
    typedef vector< vector< vector< double > > > vector3d; 
    // Storage space for the table
    vector3d xField;
    vector3d yField;
    vector3d zField;
    // The dimensions of the table
    int nx,ny,nz; 
    // The physical limits of the defined region
    double minx, maxx, miny, maxy, minz, maxz;
    // The physical extent of the defined region
    bool invertX, invertY, invertZ;
    // region size
    double dx, dy, dz;
    
public:
    STTabulatedField3D(const char* filename, 
                       double xOff, double yOff, double zOff);
    STTabulatedField3D();
    
    // GetFieldValue is for use by Geant4
    void GetFieldValue(const double Point[4], double *Bfield) const;
    // GetField prints the entire map to screen
    void GetField();
    // this version saves the map to a file
    void GetField(const char* filename);
    // Set a new field map, clears any previous field and runs init()
    void SetFieldMap(const char* filename);
    // Setters for the Offsets
    void SetXoffset(double xOff) {xOffset = xOff;}
    void SetYoffset(double yOff) {yOffset = yOff;}
    void SetZoffset(double zOff) {zOffset = zOff;}
    // Setter for default flat
    void SetDefaultsFlag(bool flag) {defaultFlag = flag;}
    
};

#endif