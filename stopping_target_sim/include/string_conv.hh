/*
 *  string_conv.hh
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 05/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#ifndef STRINGCONV_HH
#define STRINGCONV_HH

#include "string_conv.hh"

#include <string>

using namespace std;

int string_to_int(string str);
float string_to_float(string str);
bool str_is_number(string str);

inline bool is_not_number(char c) {return !(isdigit(c) || c == '.' || c == '-');}

#endif