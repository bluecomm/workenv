import os
import sys

# FCAspNormal_20170406_size=534G -> 
#  FCAspNormal 20170406 size 534G
def get_items(line):
    datasource, value = line.split("=")
    datasource, date, target = datasource.split("_")
    return datasource, date, target, value

def get_contant(filename):
    contant_dict = {}
    # contant_dict["size"] = {}
    # contant_dict["count"] = {}
    # contant_dict["finish"] = {}
    for line_origin in open(filename):
        line = line_origin.strip('\r').strip('\n')
        datasource, date, target, value = get_items(line)
        if target not in contant_dict:
            contant_dict[target] = {}
        if date not in contant_dict[target]:
            contant_dict[target][date] = {}
        if datasource not in contant_dict[target][date]:
            contant_dict[target][date][datasource] = ""
        contant_dict[target][date][datasource] = value
    
    return contant_dict

def generate_table(contant_dict):
    table_contant = ""
    # targets = contant_dict.keys()
    targets = ["size", "count", "finish"]
    dates = sorted(contant_dict[targets[0]].keys())
    dates.reverse()
    # datasources = sorted(contant_dict[targets[0]][dates[0]].keys())
    datasources = ['FCAspNormal', 'FCCharge', 'FCRebate', 'BabylonCharge', 'BabylonRebate']
    # write table title
    table_contant += "<tr>\n<td> </td>\n<td>Date</td>\n"
    for datasource in datasources:
        table_contant += "<td>" + datasource + "</td>\n"
    table_contant += "</tr>\n"
    for target in targets:
        table_contant += "<tr>\n"
        # first row
        table_contant += "<td rowspan=\"" + str(len(dates)) + "\">" + target + "</td>\n"
        date = dates[0]
        table_contant += "<td>" + date + "</td>\n"
        for datasource in datasources:
            table_contant += "<td>" + contant_dict[target][date][datasource] + "</td>\n"
        table_contant += "</tr>\n"
        # other rows
        for date in dates[1:]:
            table_contant += "<tr>\n"
            table_contant += "<td>" + date + "</td>\n"
            for datasource in datasources:
                table_contant += "<td>" + contant_dict[target][date][datasource] + "</td>\n"
            table_contant += "</tr>\n"
    return table_contant

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "please input filename"
        exit(1)
    filename = sys.argv[1]
    contant_dict = get_contant(filename)
    print contant_dict
    targets = contant_dict.keys()
    print targets
    dates = sorted(contant_dict[targets[0]].keys())
    print dates
    datasources = sorted(contant_dict[targets[0]][dates[0]].keys())
    print datasources
    
    table_contant = generate_table(contant_dict)
    print table_contant
