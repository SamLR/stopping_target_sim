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

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

void setOffsetCmds(STTabulatedField3DMessenger *msngr, // the messenger
                   G4UIcmdWithADoubleAndUnit *&cmd, // the command
                   char label) // x, y or z
{      
    // the strings that need editing
    char location[11]; char guidance [40]; char paramName[8];
    sprintf(location, "ST/%cOffset", label);
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
    
    printMapCmd_m = new G4UIcmdWithABool("ST/getField", this);
    char getFieldMsg[] = "Prints the B-field to screen, format x y z Bx By Bz Bmod";
    printMapCmd_m->SetGuidance(getFieldMsg);
    printMapCmd_m->SetDefaultValue(false);
//    printFieldCmd_m->availabelStateList // probably should be set
    
    saveMapToFileCmd_m = new G4UIcmdWithAString("ST/saveField", this);
    char setFieldMsg[] = "Saves the B-field to specified file fmt: x y z Bx By Bz Bmod";
    saveMapToFileCmd_m->SetGuidance(setFieldMsg);
    saveMapToFileCmd_m->SetDefaultValue("");
    
    setMapFileCmd_m = new G4UIcmdWithAString("ST/fieldFile", this);
    char setFieldInMsg[] = "The file to be read in as a field fmt: x y z Bx By Bz";
    setMapFileCmd_m->SetGuidance(setFieldInMsg);
    setMapFileCmd_m->SetDefaultValue("input/bfield_roi.table");
    
    verbosityCmd_m = new G4UIcmdWithAnInteger("ST/verbose", this);
    verbosityCmd_m->SetGuidance("How much information to print");
    verbosityCmd_m->SetParameterName("verbose", true);
    verbosityCmd_m->SetDefaultValue(1);
    
    setOffsetCmds(this, setXoffsetCmd_m, 'x');
    setOffsetCmds(this, setXoffsetCmd_m, 'y');
    setOffsetCmds(this, setXoffsetCmd_m, 'z');
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

void STTabulatedField3DMessenger::setNewValue(G4UIcommand* cmd, G4String newVal) 
{
    if (cmd == printMapCmd_m ) {
        if (printMapCmd_m->GetNewBoolValue(newVal)) {
            field_m->GetField();
        }
    } 
    else if (cmd == saveMapToFileCmd_m) {
        field_m->GetField(newVal);
    }
    else if (cmd == verbosityCmd_m) {
        field_m->SetVerbosity(verbosityCmd_m->GetNewIntValue(newVal));
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
