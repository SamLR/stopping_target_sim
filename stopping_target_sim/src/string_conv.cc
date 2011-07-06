/*
 *  string_conv.cc
 *  stopping_target_sim
 *
 *  Created by Sam Cook on 05/07/2011.
 *  Copyright 2011 UCL. All rights reserved.
 *
 */

#include "string_conv.hh"


#include <stdlib.h>
#include <ctype.h>
#include <algorithm>

int string_to_int(string str)
{
//    if (!str_is_number(str)) return 0;
    return atoi(str.c_str());
}

float string_to_float(string str)
{
//    if (!str_is_number(str)) return 0;
    return atof(str.c_str());
}

bool str_is_number(string str)
{
    // returns true if str is only digits and '.'s; 
    // find_if iterates through the str and finds the first instance
    // where is_not_number==true if not returns end of string
    return find_if(str.begin(), str.end(), is_not_number) == str.end();
}
