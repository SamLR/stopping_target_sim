#! /usr/bin/python

def main():
    file_name = "music_particle_dist.txt"
    # file_name = "stopping_target_sim/test_particles.txt"                             
    positions = []
    momentums = []
    
    entry_count = 0

    with open(file_name, "r") as file_in:
        for line in file_in:
            bits = line.split()
            bits = map(float, bits) # convert it all to floats
            positions.append(bits[2:5] )
            momentums.append(bits[5:])
            entry_count += 1
                    
    positions = index_list(positions)
    momentums = index_list(momentums)
    print "==== POSITIONS ===="
    print "average position is: (",
    print_func(positions, lambda l: float(sum(l))/len(l) ) # average
    print ")\nMin position is: (",
    print_func(positions, min)
    print ")\nMax position is: (",
    print_func(positions, max)
    print ")"
    
    print "\n\n==== MOMENTUM ===="
    print "average momentum is: (",
    print_func(momentums, lambda l: float(sum(l))/len(l) ) # average
    print ")\nMin momentum is: (",
    print_func(momentums, min)
    print ")\nMax momentum is: (",
    print_func(momentums, max)
    print ")"


def print_func(l, func):
    for i in l: 
        res = func(i)
        print "%+.2f,"%res,

            
def index_list(l, res=None):
    """
    Returns a list of items in l sorted by 
    their index within the nest they occupy"""
    res = [] if (res == None) else res
    for i in l:
        if hasattr(i, "__iter__"):
            index_list(i, res)
        else:
            index = l.index(i)
            if (index < len(res)):
                res[index].append(i) # add the item to the list
            else:
                res.append([i]) # make a new list
    return res


def test_index_list():
    tests = (((1,2,3), (3,4,5)), 
             (1, 2, 3, (4,5,6)),
             ((1,2,3), 4, 5, 6),
             ((1,2), 3, (4,5,6)),
             ('abit', ('long','this')),
             )
    for test in tests:
        print "\n**** TEST %i ****"%tests.index(test)
        print "list: ", test
        print "result: ", index_list(test)
    


if __name__=="__main__":
    main()
    # test_index_list()