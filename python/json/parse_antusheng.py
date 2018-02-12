import json

input_file = "input_antusheng.txt"

# read json
f = open(input_file)
line_origin = f.read()
line = line_origin.strip('\r').strip('\n')
json_obj = json.loads(line)
print "*****************************************"
print json.dumps(json_obj, indent=4)

print "*****************************************"
for itemm in json_obj['item']:
    print itemm['url'],itemm['height']
print json_obj["item"][0]
print json_obj['title']
print "*****************************************"

# add
json_obj['item'].append({"height":111,"url":"blueblue.online"})
json_obj["more_filed"] = "added_field"
# update
json_obj['title'] = "changed_title"
json_obj['user_name'] = "changed_username"
# deldete
json_obj['item'].pop(0)

# write json
print json.dumps(json_obj, indent=4)
print "*****************************************"
