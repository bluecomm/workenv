import json

input_file = "input_json.txt"

for line_origin in open(input_file):
    line = line_origin.strip('\r').strip('\n')
    json_obj = json.loads(line)
    READER_OPERATORS_TIME = json_obj['READER+OPERATORS']
    WRITER_TIME = json_obj['WRITER']
    print READER_OPERATORS_TIME, WRITER_TIME
