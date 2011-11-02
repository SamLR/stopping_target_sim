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
                tmp_file.write(line)
            else:
                x,y,z = line.split()[:3]
                if not isInROI(roi, (x,y,z)): 
                    continue
                else:
                    tmp_file.write(line)
                    # need to know how many values on each axis
                    if (not x in x_vals): x_vals.append(x)
                    if (not y in y_vals): y_vals.append(y)
                    if (not z in z_vals): z_vals.append(z)
    
    tmp_file.seek(0)
    
    with open(args.outfilename, 'w') as out_file:
        out_file.write("%i %i %i\n"%(len(x_vals), len(y_vals), len(z_vals)))
        
        for line in tmp_file:
            out_file.write(line)
    
    tmp_file.close()            


def isInROI(roi, position):
    """tests that the position is within the region of interest"""
    pos = dict(zip(('x','y','z'), position)) 
    for i in roi: # roi has indexs (x,y,z)
        # roi[i][0] == min; roi[i][1] = max
        inROI = (roi[i][0] < pos[i] < roi[i][1]) or \
                ((roi[i][0] < pos[i]) and (roi[i][1]==None)) or \
                ((roi[i][0] == None)  and (pos[i] < roi[i][1]))
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
    
    parser.add_argument('--ignore', dest='ignore_lines', default=9, 
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
       args.xmax, args.xmin = 399, 1401
       args.ymax, args.ymin = -1,  1101
       args.zmax, args.zmin = 3099, -4101
       # default ROI is 1x1x1m centred on (900mm, 0mm, 3600mm)
       
    return args


def testRoi():
    roi = {'x':(-10, 10),'y':(-10, 10),'z':(-10, 10),}
    f = lambda l: (l[1],l[2],l[0]) # rotate left function
    posF = (-11, 0, 0) # position that will return false 
    posF2 = ( 11, 0, 0)
    tests = [(0,0,0),(-11,-11,-11),(11,11,11)] 
    roi2 = {'x':(-10, None),'y':(-10, None),'z':(-10, None),}
    roi3 = {'x':(None, 10),'y':(None, 10),'z':(None, 10),}
    for i in range(3): # generate tests for all dimensions
        tests.append(posF)
        tests.append(posF2)
        posF=f(posF)
        posF2=f(posF2)
        
    print "tests", tests
    print "roi1", roi
    print "roi2", roi2
    print "roi3", roi3, "\n\n"
    
    for test in tests: 
        print "*"*40
        print "test" , test
        print "roi1", roi  
        print isInROI(roi,  test)
        print "roi2", roi2 
        print isInROI(roi2, test)
        print "roi3", roi3 
        print isInROI(roi3, test)
               

if __name__ == '__main__':
    main()
    # testRoi()
