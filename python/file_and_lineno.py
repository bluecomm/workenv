import sys  

def print_lineno():
    print __file__, ": ", sys._getframe().f_lineno  
    print "{0}:{1}:[{2}]".format(__file__, sys._getframe().f_lineno  , "aa")
