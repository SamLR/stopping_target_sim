//
//  STTabulatedField3DMessenger.hh
//  stopping_target_sim
//
//  Created by Sam Cook on 29/11/2011.
//  Copyright 2011 UCL. All rights reserved.
//

#ifndef STTabulatedField3DMessenger_hh
#define STTabulatedField3DMessenger_hh

#include "G4UImessenger.hh"
#include "globals.hh"


class STTabulatedField3D;

class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class STTabulatedField3DMessenger: public G4UImessenger
{
public:
    STTabulatedField3DMessenger(STTabulatedField3D*);
    ~STTabulatedField3DMessenger();
    
    void setNewValue(G4UIcommand*, G4String);
    
private:
    STTabulatedField3D* field_m;
    
    G4UIcmdWithABool*   printMapCmd_m; // prints field
    G4UIcmdWithAString* saveMapToFileCmd_m; // saves the field
    G4UIcmdWithAString* setMapFileCmd_m; // sets which map to use
    G4UIcmdWithADoubleAndUnit* setXoffsetCmd_m; // set the offsets
    G4UIcmdWithADoubleAndUnit* setYoffsetCmd_m;
    G4UIcmdWithADoubleAndUnit* setZoffsetCmd_m;

    // later set it up so that can generate the bfieldFile internally
    
};


#endif
