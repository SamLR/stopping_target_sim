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

class STTabulatedField3DMessenger: public G4UImessenger
{
public:
    STTabulatedField3DMessenger(STTabulatedField3D*);
    ~STTabulatedField3DMessenger();
    
    void setNewValue(G4UIcommand*, G4String);
    
private:
    STTabulatedField3D* field_m;
    
    G4UIcmdWithABool*   getFieldCmd;
    G4UIcmdWithAString* setFieldOutFileCmd;
    G4UIcmdWithAString* setFieldInFileCmd;
    // later set it up so that can generate the bfieldFile internally
    
};


#endif
