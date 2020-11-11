import threading
import time

exitFlag = 0

class myThread (threading.Thread):
    def __init__(self, threadID, name, delay):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.delay = delay
    def run(self):
        #print (self.name + " start")
        print_time(self.name, self.delay)
        #print (self.name + " end")

def print_time(threadName, delay):
    time.sleep(delay)
    print ("%s: %u / %s" % (threadName, delay, time.ctime(time.time())))

list_to_sort = [1,9,4,19,3,13,2,11,23,17,8,16,12]
list_threads = []
thread_num = 1
for lts in list_to_sort:
    list_threads.append(myThread(thread_num, "Thread-{i}".format(i=str(thread_num)), lts))
    thread_num += 1

for th in list_threads:
    th.start()

for th in list_threads:
    th.join()

print("All threads exit")
