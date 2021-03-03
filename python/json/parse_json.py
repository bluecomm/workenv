import json

input_file = "input_json.txt"

count = 0
time_dict = {}

for line_origin in open(input_file):
    line = line_origin.strip('\r').strip('\n')
    json_obj = json.loads(line)
    for k,v in json_obj.items():
        if k in time_dict:
            time_dict[k] += float(v)
        else:
            time_dict[k] = float(v)

    count += 1

print('count: ' + str(count))
print("\n******* TOTOAL **********")
for k,v in time_dict.items():
    print("{k}: \t{v}".format(k=k,v=v))

print ("\n******* AVG **********")
for k,v in time_dict.items():
    print("{k}: \t{v}".format(k=k,v=str(v/count)))

#    READER_OPERATORS_TIME = json_obj['READER+OPERATORS']
#    WRITER_TIME = json_obj['WRITER']
#    print READER_OPERATORS_TIME, WRITER_TIME
