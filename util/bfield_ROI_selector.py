#!/usr/bin/python
# encoding: utf-8
"""
bfield_ROI_selector.py

Created by Sam Cook on 2011-10-31.
"""

import argparse
from tempfile import TemporaryFile

def main():
    args = getArgs()
    x_vals, y_vals, z_vals = [], [], []
    tmp_file = TemporaryFile()
    first_line = True
    roi = {'x':(args.xmin, args.xmax), 
           'y':(args.ymin, args.ymax), 
           'z':(args.zmin, args.zmax)}
    line_count = 0
    with open(args.infilename, 'r') as in_file:
        for line in in_file:
            line_count += 1
            if first_line: 
                first_line = False # first line is one we want to re-write
                continue
            elif line_count < args.ignore_lines:
                continue
                tmp_file.write(line)
            else:  
                position = line.split()[:3]
                position = [float(i) for i in position]
                if not isInROI(roi, position): 
                    # print 'BAD LINE line', line, 'xyz', x, y, z
                    continue
                else:
                    # print 'GOOD LINE line', line, 'xyz', x, y, z
                    tmp_file.write(line)
                    # need to know how many values on each axis
                    if (not position[0] in x_vals): x_vals.append(position[0])
                    if (not position[1] in y_vals): y_vals.append(position[1])
                    if (not position[2] in z_vals): z_vals.append(position[2])
    
    tmp_file.seek(0)
    
    with open(args.outfilename, 'w') as out_file:
        out_file.write("%i %i %i\n"%(len(x_vals), len(y_vals), len(z_vals)))
        for line in tmp_file:
            out_file.write(line)
        tmp_file.close()            
    print x_vals
    print y_vals
    print z_vals
    print "stats"
    print " |   min   |   max   |   average "
    print "x| %7i | %7i | %5.1f"%minMaxAver(x_vals)
    print "y| %7i | %7i | %5.1f"%minMaxAver(y_vals)
    print "z| %7i | %7i | %5.1f"%minMaxAver(z_vals)

def minMaxAver(vals):
    return min(vals), max(vals), sum(vals)/len(vals)


def isInROI(roi, position):
    """tests that the position is within the region of interest"""
    pos = dict(zip(('x','y','z'), position)) 
    for i in roi: # roi has indexs (x,y,z)
        min, max = roi[i][0], roi[i][1]
        
        inROI = ( min < pos[i] < max) or \
                ((min < pos[i]) and (max==None)) or \
                ((min == None ) and (pos[i] < max))
                # test between min and max, or >/< min/max if other is "None"
        if not inROI: return False
    return True
    


def getArgs():
    description="scans a magnetic field map and selects a region of interest, "
    
    parser = argparse.ArgumentParser(description=description)
    
    parser.add_argument('--xup',   dest='xmax', 
                        default=None, type=float, help='max x-val')
    parser.add_argument('--xdown', dest='xmin', 
                        default=None, type=float, help='min x-val')    
    parser.add_argument('--yup',   dest='ymax', 
                        default=None, type=float, help='max y-val')
    parser.add_argument('--ydown', dest='ymin', 
                        default=None, type=float, help='min y-val')    
    parser.add_argument('--zup',   dest='zmax', 
                        default=None, type=float, help='max z-val')
    parser.add_argument('--zdown', dest='zmin', 
                        default=None, type=float, help='min z-val')
    
    parser.add_argument('--ignore', dest='ignore_lines', default=10, 
                        type=int, help='number of lines not to test')
    
    
    parser.add_argument('--infile', dest='infilename', 
                        default='../../MUSIC-3D-filed-map/MuSIC_bfield.table',
                        help='the input file to read')
                        
    parser.add_argument('--outfile', dest='outfilename', 
                        default='bfield_roi.table',
                        help='the output file to write')
    args = parser.parse_args()
    if args.xmax == None and args.xmin == None and \
            args.ymax == None and args.ymin == None and \
            args.zmax == None and args.zmin == None:
       args.xmin, args.xmax = 399,  1401
       args.ymin, args.ymax = -1,   1001
       args.zmin, args.zmax = 3099, 4101
       # default ROI is 1x1x1m centred on (900mm, 0mm, 3600mm)
       
    return args


def testRoi():
    roi = {'x':(10, 20), 'y':(10, 20), 'z':(10, 20)}
    in_roi = ((11, 12, 13), (11, 11, 11), (19, 19, 19))
    not_in_roi = ((11,11,30), (11, 30, 11), (30,11,11)) 
    print "in ROI"
    for i in in_roi: print isInROI(roi, i)
    print "not in ROI"
    for i in not_in_roi: print isInROI(roi, i)
    
               


if __name__ == '__main__':
    main()
    # testRoi()
