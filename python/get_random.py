import sys
import random
import time
import datetime

def get_random_int(low=1, hight=10, num=10000):
    result = {}
    for i in range(low, hight + 1):
        result[i] = 0
    seed_list = [low, hight, time.time()]
    random.seed(id(seed_list))
    for i in range(10000):
        rand_int = random.randint(low, hight)
        result[rand_int] += 1
    return result

 
if __name__ == "__main__":
    now_time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print(now_time)
    low = 1
    hight = 10
    if len(sys.argv) == 2:
        hight = int(sys.argv[1])
    elif len(sys.argv) >= 3:
        low = int(sys.argv[1])
        hight = int(sys.argv[2])

    result = get_random_int(low, hight)
    result_sorted = sorted(result.items(), key = lambda x:x[1], reverse = True)
    for item in result_sorted:
        print(item)

