#!/usr/bin/python

# get the number of particles to process
import argparse
import sys

description = """Prints the lines from a column based file who's position
              fall between a min and max. Used to extract subsets of
              values from data files (eg particles)."""

parser = argparse.ArgumentParser(description=description)

parser.add_argument('-n', dest='n_lines', type=int, default=None, 
                    help='number of lines to print')
                    
parser.add_argument('-c', dest='col', default=2, type=int,
                    help='column number (from 0) in which to find x')
                    
parser.add_argument('-l', dest='ignore_lines', default=0, type=int,
                    help='number of lines to ignore from the start of the file')

parser.add_argument('-lp', dest='print_ignore_lines', default=True, type=bool,
                    help='whether to print ignored lines')

parser.add_argument('--file', dest='file_name', type=file, 
                    default='../../music_particle_dist.txt',
                    help='the source file for particles')
                    
parser.add_argument('--max', dest='max', nargs=3, default=[1000., 1000., 2800.], 
                    type=float, metavar="X",
                    help='maximum values (x, y z)')
                    
parser.add_argument('--min', dest='min', nargs=3, default=[-1000., -1000., 2700],
                    type=float, metavar="X", 
                    help='minimum values (x, y z)')

args = parser.parse_args()

pids = {}

for min, max in zip(args.min, args.max):
    if (min >= max):
        print "error: min (%.1f) greater than or equal to max (%.1f)"%(min, max)
        sys.exit(1)    

for line in args.file_name:
    if (args.ignore_lines > 0): # skip header lines
        if (args.print_ignore_lines): print line, 
        args.ignore_lines -=1
        continue
    split = line.split()
    x, y, z = map(float, split[args.col: args.col+3])
    # check that the point is within the bounds
    if not (args.min[0] < x < args.max[0]) or \
       not (args.min[1] < y < args.max[1]) or \
       not (args.min[2] < z < args.max[2]): continue
    
    if (not split[1] in pids):
        pids[split[1]] = 1
    else:
        pids[split[1]] += 1
    
    print line,
    if (args.n_lines):
            args.n_lines -= 1
            if (args.n_lines <= 0): break

for k,v in pids.items():
    print "%12.0f %6.0f"%(float(k),float(v))