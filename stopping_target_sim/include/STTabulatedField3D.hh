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

using namespace std;

class STTabulatedField3D: public G4MagneticField
{
    typedef vector< vector< vector< double > > > vector3d; // vector with 3 subscripts 
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
    double dx, dy, dz;
    double fZoffset;
    
public:
    STTabulatedField3D(const char* filename, double zOffset );
    void GetFieldValue(const double Point[4], double *Bfield) const;
};

#endif