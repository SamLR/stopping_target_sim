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

STTabulatedField3DMessenger::STTabulatedField3DMessenger(STTabulatedField3D* field)
: 