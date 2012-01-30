//
//  STTabulatedField3DMessenger.cc
//  stopping_target_sim
//
//  Created by Sam Cook on 29/11/2011.
//  Copyright 2011 UCL. All rights reserved.
//

#include <iostream>

#include "STTabulatedField3DMessenger.hh"
#include "STTabulatedField3D.hh"

#include "G4UIdirectory.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

void setOffsetCmds(STTabulatedField3DMessenger *msngr, // the messenger
                   G4UIcmdWithADoubleAndUnit *&cmd, // the command
                   char label) // x, y or z
{      
    // the strings that need editing
    char location[12]; char guidance [40]; char paramName[8];
    sprintf(location, "/ST/%cOffset", label);
    sprintf(guidance, "%c offset of the magnetic field map", label);
    sprintf(paramName, "%cOffset", label);
    // create the new command
    cmd = new G4UIcmdWithADoubleAndUnit(location, msngr);
    cmd->SetGuidance(guidance);
    cmd->SetParameterName(paramName, true);
    cmd->SetDefaultUnit("mm");

}

STTabulatedField3DMessenger::STTabulatedField3DMessenger(STTabulatedField3D* field)
: field_m(field) 
{
    stDir_m = new G4UIdirectory("/ST/");
    stDir_m->SetGuidance("Custom commands");
    
    printMapCmd_m = new G4UIcmdWithoutParameter("/ST/getField", this);
    char getFieldMsg[] = "Prints the B-field to screen, format x y z Bx By Bz Bmod";
    printMapCmd_m->SetGuidance(getFieldMsg);

    printInterpMapCmd_m = new G4UIcmdWithoutParameter("/ST/getInterpolatedField", this);
    char getIntFieldMsg[] = "Prints the interpolated B-field to screen, format x y z Bx By Bz Bmod";
    printInterpMapCmd_m ->SetGuidance(getIntFieldMsg);
    
    saveMapToFileCmd_m = new G4UIcmdWithAString("/ST/saveField", this);
    char setFieldMsg[] = "Saves the B-field to specified file fmt: x y z Bx By Bz Bmod";
    saveMapToFileCmd_m->SetGuidance(setFieldMsg);
    saveMapToFileCmd_m->SetDefaultValue("");
    
    saveInterpMapToFileCmd_m = new G4UIcmdWithAString("/ST/saveInterpolatedField", this);
    char setIntFieldMsg[] = "Saves the interpolated B-field to specified file fmt: x y z Bx By Bz Bmod";
    saveInterpMapToFileCmd_m->SetGuidance(setIntFieldMsg);
    saveInterpMapToFileCmd_m->SetDefaultValue("");
    
    updateCmd_m = new G4UIcmdWithoutParameter("/ST/update", this);
    char updateMsg[] = "Clears and re-initialise the field map";
    updateCmd_m->SetGuidance(updateMsg);
    
    setMapFileCmd_m = new G4UIcmdWithAString("/ST/setFieldFile", this);
    char setFieldInMsg[] = "The file to be read in as a field fmt: x y z Bx By Bz";
    setMapFileCmd_m->SetGuidance(setFieldInMsg);
    setMapFileCmd_m->SetDefaultValue("input/bfield_roi.table");
    
    verbosityCmd_m = new G4UIcmdWithAnInteger("/ST/verbose", this);
    verbosityCmd_m->SetGuidance("How much information to print");
    verbosityCmd_m->SetParameterName("verbose", true);
    verbosityCmd_m->SetDefaultValue(1);
    
    setOffsetCmds(this, setXoffsetCmd_m, 'x');
    setOffsetCmds(this, setYoffsetCmd_m, 'y');
    setOffsetCmds(this, setZoffsetCmd_m, 'z');
}

STTabulatedField3DMessenger::~STTabulatedField3DMessenger() 
{
    delete printMapCmd_m;
    delete setMapFileCmd_m;
    delete saveMapToFileCmd_m;
    delete verbosityCmd_m;
    delete setXoffsetCmd_m;
    delete setYoffsetCmd_m;
    delete setZoffsetCmd_m;
}

void STTabulatedField3DMessenger::SetNewValue(G4UIcommand* cmd, G4String newVal) 
{
    if (cmd == printMapCmd_m ) {
        field_m->GetField(false);
    } 
    else if (cmd == printInterpMapCmd_m) {
        field_m->GetField(true);
    }
    else if (cmd == saveMapToFileCmd_m) {
        field_m->GetField(newVal, false);
    }
    else if (cmd == saveInterpMapToFileCmd_m) {
        field_m->GetField(newVal, true);
    }
    else if (cmd == verbosityCmd_m) {
        field_m->SetVerbosity(verbosityCmd_m->GetNewIntValue(newVal));
    } 
    else if (cmd == updateCmd_m) {
        field_m->update();
    }
    else if (cmd == setMapFileCmd_m) {
        field_m->SetFieldMap(newVal);
        field_m->SetDefaultsFlag(false);
    }
    else if (cmd == setXoffsetCmd_m) {
        field_m->SetXoffset(setXoffsetCmd_m->GetNewDoubleValue(newVal));
        field_m->SetDefaultsFlag(false);
    } 
    else if (cmd == setYoffsetCmd_m) {
        field_m->SetYoffset(setYoffsetCmd_m->GetNewDoubleValue(newVal));
        field_m->SetDefaultsFlag(false);
    } 
    else if (cmd == setZoffsetCmd_m) {
        field_m->SetZoffset(setZoffsetCmd_m->GetNewDoubleValue(newVal));
        field_m->SetDefaultsFlag(false);
    }
}