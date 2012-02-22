/*
 *  STTabulatedField3D.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 21/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#include "STTabulatedField3D.hh"
#include "STTabulatedField3DMessenger.hh"

#include <fstream>
#include <cmath>

double getMod(const double in_vector [3])
{
    // calculates the modulous of in_vector
    double sqs [3];
    for (int i = 0 ; i < 3; ++i) 
    {
        sqs [i] = in_vector[i] * in_vector[i];
    }
    return std::sqrt(sqs[0] + sqs[1] + sqs[2]);
}

double getMod(double x, double y, double z)
{
    double position [] = {x, y, z};
    return getMod(position);
}

STTabulatedField3D::STTabulatedField3D(const char* filename, 
                                       double xOff, double yOff, double zOff) 
:xOffset(xOff), yOffset(yOff), zOffset(zOff), verbose(3), defaultsUsed(true)
{
    messenger_m = new STTabulatedField3DMessenger(this);
    sprintf(filename_m, "%s", filename);
    Init();
}

STTabulatedField3D::STTabulatedField3D() 
: verbose(3), defaultsUsed(true), xOffset(-1010.99*mm), yOffset(0), zOffset(-3778.43*mm)
{    
    //    sprintf(filename_m, "/Users/samcook/code/MuSIC/MuSIC_simulation/stopping_target_sim/input/Bfield_roi.table");// default    
    sprintf(filename_m, "../input/Bfield_roi.table");// default
    
    messenger_m = new STTabulatedField3DMessenger(this);
    if (defaultsUsed and verbose) {
        G4cout << "\n-----------------------------------------------------------"
               << "\n   USING DEFAULT MAGNETIC FIELD SETTINGS"
               << "\n-----------------------------------------------------------" 
               << G4endl;
    }
    Init();
}

void STTabulatedField3D::Init()
{
    needUpdate = false;
    if (!defaultsUsed) {
        G4cout<< "\n-----------------------------------------------------------"
              << "\n   UPDATING FIELD SETTINGS"
              << "\n -----------------------------------------------------------" 
              << G4endl;
    }
    double lenUnit= mm;
    double fieldUnit= tesla; 
    if (verbose > 1) {    
        G4cout << "\n-----------------------------------------------------------"
               << "\n      Magnetic field"
               << "\n-----------------------------------------------------------";
        
        G4cout << "\n ---> " "Reading the field map from " 
               << filename_m << " ... " << endl; 
        
        G4cout << "\n Offsets:"
               << "\n x: " << xOffset << "mm"
               << "\n y: " << yOffset << "mm"
               << "\n z: " << zOffset << "mm" <<G4endl;
    }
    ifstream file( filename_m ); // Open the file for reading.

    if (!file.good())
    {
        G4cout << "File not properly opened quiting" << endl;
        exit(0);
    } 
    // Read table dimensions 
    file >> nx >> ny >> nz;
    if (verbose > 1) {
        G4cout << "  [ Number of values x,y,z: " 
               << nx << " " << ny << " " << nz << " ] "
               << endl;
    }
    
    // Set up storage space for table
    xField.resize( nx );
    yField.resize( nx ); // swap is applied in 'getFieldValue'?
//        yField.resize( 2*nx ); // assume field vertically symmetric hence double 
    zField.resize( nx );
    int ix, iy, iz;
    for (ix=0; ix<nx; ix++) {
        xField[ix].resize(ny);
        yField[ix].resize(ny);
        zField[ix].resize(ny);
        for (iy=0; iy<ny; iy++) {
            xField[ix][iy].resize(nz);
            yField[ix][iy].resize(nz);
            zField[ix][iy].resize(nz);
        }
    }

    // Ignore other header information    
    // The first line whose second character is '0' is considered to
    // be the last line of the header.
    // the above is a stupid rule but whatever, that's how they seem to be made
    // buffer+getline method used due to non-consistent structure of header
    char buffer[256];
    do {
        file.getline(buffer,256);
    } while ( buffer[1]!='0');

    // Read in the data
    double xval, yval, zval, bx, by, bz, bmod;
    
    for (ix=0; ix<nx; ix++) {
        for (iy=0; iy<ny; iy++) {
            for (iz=0; iz<nz; iz++) {
                // begin the looping
                file >> xval >> yval >> zval >> bx >> by >> bz >> bmod;

                // assume sanely ordered table i.e either min or max @ start
                if ( ix==0 && iy==0 && iz==0 ) { 
//                    minx = -1 * (xval * lenUnit + xOffset); // x is flipped
                    minx =       xval * lenUnit + xOffset; // x is flipped
                    miny =       yval * lenUnit + yOffset;
                    minz =       zval * lenUnit + zOffset;
                }
                xField[ix][iy][iz] = bx * fieldUnit;
                yField[ix][iy][iz] = by * fieldUnit;
                zField[ix][iy][iz] = bz * fieldUnit;
            }
        }
    }
    file.close();
    // assume final values are either min or max
//    maxx = -1*(xval * lenUnit + xOffset);
    maxx =    (xval * lenUnit + xOffset);
    maxy =     yval * lenUnit + yOffset;
    maxz =     zval * lenUnit + zOffset;
    
    if (verbose > 1) {
        G4cout << "\n ---> ... done reading " 
               << "\n ---> assumed the order:  x, y, z, Bx, By, Bz "
               << "\n ---> Min position x,y,z: " 
               << minx/mm << " " << miny/mm << " " << minz/mm << " mm "
               << "\n ---> Max position x,y,z: " 
               << maxx/mm << " " << maxy/mm << " " << maxz/mm << " mm "
               << "\n ---> The field will be offset by " 
               << "\n x: "<< xOffset/mm << " mm "
               << "\n y: "<< yOffset/mm << " mm "
               << "\n z: "<< zOffset/mm << " mm "<< endl;
    }
    // reset assumed state
    invertX = false;
    invertY = false;
    invertZ = false;
    // Should really check that the limits are not the wrong way around.
    if (maxx < minx) {swap(maxx,minx); invertX = true;} 
    if (maxy < miny) {swap(maxy,miny); invertY = true;} 
    if (maxz < minz) {swap(maxz,minz); invertZ = true;} 
    
    if (verbose > 1) {
        G4cout << "\nAfter reordering if neccesary"  
               << "\n ---> Min values x,y,z: " 
               << minx/mm << " " << miny/mm << " " << minz/mm << " mm "
               << "\n ---> Max values x,y,z: " 
               << maxx/mm << " " << maxy/mm << " " << maxz/mm << " mm ";
    }
    
    dx = maxx - minx;
    dy = maxy - miny;
    dz = maxz - minz;
    
    if (verbose > 1) {
        G4cout << "\n ---> Dif values x,y,z (range): " 
               << dx/mm << " " << dy/mm << " " << dz/mm << " mm in z "
               << "\n-----------------------------------------------------------" 
               << endl;
    }
}

void STTabulatedField3D::GetFieldValue(const double point[4], 
                                       double *Bfield) const
{
    if (verbose > 0 and needUpdate) {
        G4cout << "\n-----------------------------------------------------------"
               << "\n WARNING: UNINITIALISED CHANGES"
               << "\n use '/ST/update' to initialise"
               << "\n-----------------------------------------------------------"
               << G4endl;
    }
    double x = point[0];
    double y = point[1];
    double z = point[2];
    // assume point[3] is for time varying fields
    
    if (y < 0) y = -y; // y is only defined for y > 0
    
    // Check that the point is within the defined region 
    if ( x>=minx && x<=maxx &&
        y>=miny && y<=maxy && 
        z>=minz && z<=maxz ) {
        
        // Position of given point within region, normalized to the range
        // [0,1]
        double xfraction = (x - minx) / dx;
        double yfraction = (y - miny) / dy; 
        double zfraction = (z - minz) / dz;
        
        if (invertX) { xfraction = 1 - xfraction;}
        if (invertY) { yfraction = 1 - yfraction;}
        if (invertZ) { zfraction = 1 - zfraction;}
        
        // Need addresses of these to pass to modf below.
        // modf uses its second argument as an OUTPUT argument.
        double xdindex, ydindex, zdindex;
        
        // Position of the point within the cuboid defined by the
        // nearest surrounding tabulated points
        // frac_part = modf(x, int_part) eg x = 3.1415....
        // frac_part = .1415 & int_part = 3
        double xlocal = ( std::modf(xfraction*(nx-1), &xdindex));
        double ylocal = ( std::modf(yfraction*(ny-1), &ydindex));
        double zlocal = ( std::modf(zfraction*(nz-1), &zdindex));
        
        // The indices of the nearest tabulated point whose coordinates
        // are all less than those of the given point
        int xindex = static_cast<int>(xdindex);
        int yindex = static_cast<int>(ydindex);
        int zindex = static_cast<int>(zdindex);
               
        // Full 3-dimensional version
        Bfield[0] =
        xField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
        xField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
        xField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
        xField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
        xField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
        xField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
        xField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
        xField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
        Bfield[1] =
        yField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
        yField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
        yField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
        yField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
        yField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
        yField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
        yField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
        yField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
        Bfield[2] =
        zField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
        zField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
        zField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
        zField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
        zField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
        zField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
        zField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
        zField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
        
    } else {
        Bfield[0] = 0.0;
        Bfield[1] = 0.0;
        Bfield[2] = 0.0;
    }
}

void STTabulatedField3D::GetFieldInterpolated(FILE* file)
{
    // prints the centre values of the bfield for each voxel to the specified 
    // file. It uses the getFieldValue function to get the values
    
    // '-1' to account for fence-post problem i.e there are n-1 voxels for any axis
    const double stepX = dx/(nx - 1); 
    const double stepY = dy/(ny - 1); 
    const double stepZ = dz/(nz - 1);

    if (verbose > 1){
        G4cout << "\n Field map variables:"
        << "\n StepX: " << stepX/mm << "mm"
        << "\n StepY: " << stepY/mm << "mm"
        << "\n StepZ: " << stepZ/mm << "mm"
        << "\n for ranges: "
        << "\n " << minx/mm << " < x < " << maxx/mm << "mm"
        << "\n " << miny/mm << " < y < " << maxy/mm << "mm"
        << "\n " << minz/mm << " < z < " << maxz/mm << "mm" << G4endl;
        }
    
    if (verbose > 2){
        double pos [] = {minx, miny,minz,0};
        double bfield [3];
        GetFieldValue(pos, bfield);
        G4cout << "\n~~~ DEBUG INFO:"
               << "\n~~~ Calibration point (minx, miny, minz) for interpolated"
               << "\n~~~ minx= "<< minx/mm << "mm"
               << "\n~~~ miny= "<< miny/mm << "mm"
               << "\n~~~ minz= "<< minz/mm << "mm"
               << "\n~~~ Bx = " << bfield[0]/tesla << "T"
               << "\n~~~ By = " << bfield[1]/tesla << "T"
               << "\n~~~ Bz = " << bfield[2]/tesla << "T" << G4endl;
    }
    
    for (double x = minx + (stepX/2); x <= maxx; x += stepX) 
    {
        for (double y = miny + (stepY/2); y <= maxy; y += stepY) 
        {
            for (double z = minz + (stepZ/2); z <= maxz; z += stepZ) 
            {
                double pos [] = {x, y, z, 0.0};
                double bfield [] = {0.0, 0.0, 0.0};
                GetFieldValue(pos, bfield);
                double bmod = getMod(bfield);
                char fmt [] = "%f %f %f %e %e %e %e\n";
                fprintf(file, fmt, x, y, z, 
                        bfield[0], bfield[1], bfield[2], bmod);
            }
        }
    }
}

void STTabulatedField3D::GetField(FILE* file)
{
    // prints to screen the field map in the format
    // x y z Bx By Bz Bmod
    const double stepX = dx/(nx - 1); // '-1' term should deal with fence-posting
    const double stepY = dy/(ny - 1);
    const double stepZ = dz/(nz - 1);
    if (verbose > 1){
        G4cout << "\n Field map variables:"
               << "\n StepX: " << stepX/mm << "mm"
               << "\n StepY: " << stepY/mm << "mm"
               << "\n StepZ: " << stepZ/mm << "mm"
               << "\n for ranges: "
               << "\n " << minx/mm << " < x < " << maxx/mm << "mm"
               << "\n " << miny/mm << " < y < " << maxy/mm << "mm"
               << "\n " << minz/mm << " < z < " << maxz/mm << "mm" << G4endl;
    }
    
    
    if (verbose > 2){
        double pos [] = {minx, miny,minz,0};
        double bfield [3];
        GetFieldValue(pos, bfield);
        G4cout << "\n~~~ DEBUG INFO:"
        << "\n~~~ Calibration point (minx, miny, minz) for direct access"
        << "\n~~~ minx= "<< minx/mm << "mm"
        << "\n~~~ miny= "<< miny/mm << "mm"
        << "\n~~~ minz= "<< minz/mm << "mm"
        << "\n~~~ Bx = " << xField[0][0][0]/tesla <<"T"
        << "\n~~~ By = " << yField[0][0][0]/tesla <<"T"
        << "\n~~~ Bz = " << zField[0][0][0]/tesla <<"T" << G4endl;
    }
    
    for (int ix = 0; ix < xField.size(); ++ix) 
    {
        double x = ix * stepX + minx;
        
        for (int iy = 0; iy < xField[ix].size(); ++iy) 
        {
            double y = iy * stepY + miny;
            
            for (int iz = 0; iz < xField[ix][iy].size(); ++iz) 
            {
                double z = iz * stepZ + minz;
                double bx = xField[ix][iy][iz];
                double by = yField[ix][iy][iz];
                double bz = zField[ix][iy][iz];
                double bmod = getMod(bx, by, bz);
                char fmt [] = "%f %f %f %e %e %e %e\n";
                fprintf(file, fmt, x, y, z, bx, by, bx, bmod);
            }
        }
    }
}

void STTabulatedField3D::GetField(bool interpolated)
{
    if (verbose > 1) {
        G4cout <<"\nPrinting field map (as stored) to screen"
        <<"\n--------------------------------------------------"<< G4endl;
    }
    
    if (interpolated) {
        GetFieldInterpolated(stdout);
    } else
    {
        GetField(stdout);
    }
    if (verbose > 1) {
        G4cout << "\n--------------------------------------------------"
        << "\nField map printing complete"
        << "\n--------------------------------------------------"<< G4endl;
    }
    
}

void STTabulatedField3D::GetField(const char* filename,bool interpolated)
{
    // as getField() but output is to a file instead
    FILE* pFile = fopen(filename, "w");
    if (pFile == NULL) {
        G4cout << "\n Error opening file, " << filename << " for writing, exiting." << G4endl;
        exit(1);
    }
    if (verbose > 1) {
        G4cout << "\n--------------------------------------------------"
               << "\nWriting field map to file, " << filename 
               << "\n--------------------------------------------------"<< G4endl;
    }
    
    if (interpolated){
        GetFieldInterpolated(pFile);
    } else
    {
        GetField(pFile);
    }
    fclose(pFile);
    if (verbose > 1) {
        G4cout << "\n--------------------------------------------------"
               << "\nField map printing complete"
               << "\n--------------------------------------------------"<< G4endl;
    }
}

void STTabulatedField3D::ClearField()
{
    // clear all vectors
    for (int ix=0; ix<nx; ix++) {
        for (int iy=0; iy<ny; iy++) {
            xField[ix][iy].clear();
            yField[ix][iy].clear();
            zField[ix][iy].clear();
        }
    }
    
    // reset derived values
    nx = 0; ny = 0; nz = 0; // map dimensions 
    minx = 0.0; miny = 0.0; minz = 0.0; // min & max limits
    maxx = 0.0; maxy = 0.0; maxz = 0.0;
    dx = 0.0; dy = 0.0; dz = 0.0; // step sizes
}

void STTabulatedField3D::SetFieldMap(const char *filename)
{
    sprintf(filename_m, "%s", filename);
    needUpdate = true;
}

void STTabulatedField3D::update()
{
    ClearField();
    Init();
}
