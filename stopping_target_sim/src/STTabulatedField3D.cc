/*
 *  STTabulatedField3D.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 21/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */



#include "STTabulatedField3D.hh"

#include <fstream>
#include <cmath>
//#include <iostream>
//#include <string> // check to see if G4String supports this
//#include <string.h>
//#include <ctype.h>

//#include "string_conv.hh" 

STTabulatedField3D::STTabulatedField3D( const char* filename, double zOffset ) 
:fZoffset(zOffset)
{    
    
    double lenUnit= mm;
    double fieldUnit= tesla; 
    G4cout << "\n-----------------------------------------------------------"
    << "\n      Magnetic field"
    << "\n-----------------------------------------------------------";
    
    G4cout << "\n ---> " "Reading the field grid from " 
           << filename << " ... " << endl; 
    ifstream file( filename ); // Open the file for reading.
    
    if (!file.good())
    {
        G4cout << "File not properly opened quiting" << endl;
        exit(0);
    }
    
    // Read table dimensions 
    file >> nx >> ny >> nz; 
    
    G4cout << "  [ Number of values x,y,z: " 
    << nx << " " << ny << " " << nz << " ] "
    << endl;
    
    // Set up storage space for table
    xField.resize( nx );
    yField.resize( 2*nx ); // assume field vertically symmetric so double 
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
    char buffer[256];
    do {
        file.getline(buffer,256);
    } while ( buffer[1]!='0');

    // Read in the data
    double xval,yval,zval,bx,by,bz;
    double permeability; // Not used in this example.
    for (ix=0; ix<nx; ix++) {
        for (iy=0; iy<ny; iy++) {
            for (iz=0; iz<nz; iz++) {
                file >> xval >> yval >> zval >> bx >> by >> bz >> permeability;
                zval += zOffset;
                if ( ix==0 && iy==0 && iz==0 ) {
                    minx = xval * lenUnit;
                    miny = yval * lenUnit;
                    minz = zval * lenUnit;
                }
                xField[ix][iy][iz] = bx * fieldUnit;
                yField[ix][iy][iz] = by * fieldUnit;
                zField[ix][iy][iz] = bz * fieldUnit;
            }
        }
    }
    file.close();
    
    maxx = xval * lenUnit;
    maxy = yval * lenUnit;
    maxz = zval * lenUnit;
    
    G4cout << "\n ---> ... done reading " << endl;
    
    // G4cout << " Read values of field from file " << filename << endl; 
    G4cout << " ---> assumed the order:  x, y, z, Bx, By, Bz "
    << "\n ---> Min values x,y,z: " 
    << minx/cm << " " << miny/cm << " " << minz/cm << " cm "
    << "\n ---> Max values x,y,z: " 
    << maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm "
    << "\n ---> The field will be offset by " << zOffset/cm << " cm " << endl;
    
    // Should really check that the limits are not the wrong way around.
    if (maxx < minx) {swap(maxx,minx); invertX = true;} 
    if (maxy < miny) {swap(maxy,miny); invertY = true;} 
    if (maxz < minz) {swap(maxz,minz); invertZ = true;} 
    G4cout << "\nAfter reordering if neccesary"  
    << "\n ---> Min values x,y,z: " 
    << minx/cm << " " << miny/cm << " " << minz/cm << " cm "
    << " \n ---> Max values x,y,z: " 
    << maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm ";
    
    dx = maxx - minx;
    dy = maxy - miny;
    dz = maxz - minz;
    G4cout << "\n ---> Dif values x,y,z (range): " 
    << dx/cm << " " << dy/cm << " " << dz/cm << " cm in z "
    << "\n-----------------------------------------------------------" << endl;
}

void STTabulatedField3D::GetFieldValue(const double point[4],
                                            double *Bfield ) const
{
    
    double x = point[0];
    double y = point[1];
    double z = point[2];
    
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

