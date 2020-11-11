#!/usr/bin/python3

import _thread
import time

def print_time(threadName, delay):
    time.sleep(delay)
    print("%s: %s / %s" % (threadName, str(delay), time.ctime(time.time())))

list_to_sort = [1,9,4,19,3,13,2,11,23,17,8,16,12]

print(list_to_sort)

count = 0
for lts in list_to_sort:
    count += 1
    try:
       _thread.start_new_thread(print_time, ("Thread-{i}".format(i=str(count)), lts))
    except:
       print ("Error: create thread error")

time.sleep(25)
