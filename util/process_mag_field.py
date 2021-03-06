#!/usr/bin/python

# this will check that a .table magnetic field is properly formatted

import argparse
from tempfile import TemporaryFile
from sams_utilities import rotate2dDegrees

description = """Checks & adjusts the X Y & Z header values in a .table file to match the file"""

parser = argparse.ArgumentParser(description=description)

parser.add_argument('--file', dest='filename', 
                    default='magfield.table', help='The file to process')

parser.add_argument('-c', dest='col', type=int, 
                    default=0, help='the first column of position values')

parser.add_argument('-l', dest='ignore_lines', type=int, 
                    default=10, help='how many lines of header there are')
    
args = parser.parse_args()

x_vals, y_vals, z_vals = [], [], []
tmp_file = TemporaryFile()
first_line = True

# first 'with' copies the contents of the file and removes the first line
# second 'with' writes the output file with the newly adjusted first line 
# the first line contains the number of entries for each dimension

# TODO Put into a single function

with open(args.filename, 'r') as file_in:
    line_count = 0
    for line in file_in:
        line_count += 1
        if (not first_line): tmp_file.write(line) #want to write everything but the first line
        first_line= False
        if (line_count <= args.ignore_lines):
            continue;
            # extract the x, y and z values and convert them to floats
        x, y, z = map(float, line.split()[args.col: args.col+3]) 
        if (not x in x_vals): x_vals.append(x)
        if (not y in y_vals): y_vals.append(y)
        if (not z in z_vals): z_vals.append(z)
        
tmp_file.seek(0)

mod = (-880, 0, -3565)

line_count = 0

with open(args.filename, 'w') as file_out:
    file_out.write("%i %i %i\n"%(len(x_vals), len(y_vals), len(z_vals)))
    # 
    # for line in tmp_file:
    #     file_out.write(line)
    for line in tmp_file:
        line_count += 1
        if (line_count >= args.ignore_lines): # 1st line now removed from tmp
            line = line.split() 
            # convert the co-ordinates to the local values
            line [:3]=[float(line[i]) + mod[i] for i in range(len(mod))]
            line[0], line[2] = rotate2dDegrees(line[0],line[2], 36)
            line = "%f %f %f %s %s %s %s \n" % tuple(line) 
        file_out.write(line)


        
tmp_file.close()

print "=======INFO======="
print " direction|  length  |    min    |   max"
print "    X     |  %6i  |   %6.1f  |  %6.1f"%(len(x_vals), min(x_vals), max(x_vals))
print "    Y     |  %6i  |   %6.1f  |  %6.1f"%(len(y_vals), min(y_vals), max(y_vals))
print "    Z     |  %6i  |   %6.1f  |  %6.1f"%(len(z_vals), min(z_vals), max(z_vals))