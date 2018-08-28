import sys
import random
import time
import datetime
 
'''
breif: get rand int of color ball
input:
    rand_num: num of int to generate
    rand_range: the range of int to gengerate
    rand_upper: the upper limit of rand number before modulo
output:
    rand_ints: the generated ints
'''
def get_color_int(rand_num, rand_range, rand_upper, seed_seed=None):
    ''' get seed '''
    seed_list = [rand_num, rand_range, rand_upper, time.time(), seed_seed]
    random.seed(id(seed_list))
    rand_ints = []
    while len(rand_ints) < rand_num:
        time.sleep(0.01)
        rand_int = random.randint(1, rand_upper) % rand_range + 1
        if rand_int in rand_ints:
            continue
        rand_ints.append(rand_int)
    ''' sort the results '''
    rand_ints = sorted(rand_ints)
    return rand_ints
 
if __name__ == "__main__":
    seed_seed = None
    now_time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    if len(sys.argv) > 1:
        seed_seed = sys.argv[1:]
    else:
        seed_seed = now_time
    print "seed: ", seed_seed

    rand_red = get_color_int(6, 33, pow(33, 12), seed_seed)
    rand_blue = get_color_int(1, 16, pow(2, 32), seed_seed)
    print "result: ", rand_red, rand_blue
