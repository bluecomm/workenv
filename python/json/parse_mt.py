import json

input_file = "input_mt.txt"

for line_origin in open(input_file):
    line = line_origin.strip('\r').strip('\n')
    json_obj = json.loads(line)
    print "planid", json_obj['planid']
    print "unitid", json_obj['unitid']
    print "mtid", json_obj['mtid']
    print "mtid[0]", json_obj['mtid'][0]
    print "it -> in_a", json_obj['it']['in_a']
