# -*- coding: GBK -*-
"""
    @author     : liding01
    @date       : Thu 22 Aug 2013 16:39:00 PM CST
    @last update: Thu 22 Aug 2013 16:39:00 PM CST
    @summary    : checker use to check log and other output
    @version    : 1.0.0.0
"""

import sys,os,getopt,time,datetime
from xml.dom.minidom import parseString,Element
import operator



# root path
#monitor_path = "/home/work/tm/majing05/monitor/"
monitor_path = "./"
# input tasks
fetcher_token_xml="data/fetcher_token.xml"
asp_bundler_token_xml="data/asp_bundler.xml"
# output html
fetcher_output_html = "fetcher.html"
asp_bundler_output_html = "bundler.html"

# cxolor for diffrent delay value
def get_tr_bgcolor(delay):
    bgcolor = "#02C700"
    return bgcolor
    
def write_page_header():
    output = ""
    output += """<html>
    <head>
    <link rel="stylesheet" type="text/css" href="css/tb.css"/>
    </head>
    <body>
    <h2>Built At: """
    output += timestamp_datetime(time.time())
    output += """</h2>\n"""
    output += """<h4><a href=\"""" + fetcher_output_html + """\">Fetcher</a>\n"""
    output += """<h4><a href=\"""" + asp_bundler_output_html + """\">Bundler</a></h4>\n"""
    return output

def write_tail():
    output = ""
    output += """
    </body>\n</html>
    """
    return output

'''
tasks: tasks to show in table
group_without: group to donnot show in table
'''
def write_fetcher_table(tasks, token_file):
    output = ""
    output += """
    <table border="1" bordercolor="#000000" style="text-align:center;">
    <tr>
    <td>Name</td>
    <td>Current</td>
    <td>Delay (min)</td>
    <td>Alarm (min)</td>
    </tr>
    """
    time_now = int(os.path.getctime(token_file))
    tasks_simple = []
    for t in tasks:
        v = t["value"]
        token_name = v["name"].strip('\"')
        token_start = v["state"]["start"]
        token_delay = int(time_now) - int(v["state"]["start"]) - int(v["source"]["delay"])
        token_delay = max(0, token_delay)
        token_alarm = int(time_now) - int(v["state"]["start"]) - int(v["source"]["alarm"])
        token_alarm = max(0, token_alarm)
        tasks_simple.append([token_name, token_start, token_delay, token_alarm])
    tasks_sorted = sorted(tasks_simple, key=operator.itemgetter(0, 1))

    for t in tasks_sorted:
        token_name = t[0]
        token_start = t[1]
        token_delay = t[2]
        token_alarm = t[3]
        tr_bgcolor = get_tr_bgcolor(max(0, token_delay / 60))
        output += "<tr bgcolor=" + tr_bgcolor + ">\n"
        output += "<td>" + token_name + "</td>\n"
        output += "<td>" + timestamp_datetime(token_start) + "</td>\n"
        output += "<td>" + str(max(0, token_delay / 60)) + "</td>\n"
        output += "<td>" + str(max(0, token_alarm / 60)) + "</td>\n"
        output += "</tr>\n"
    output += """\n</table>\n"""
    return output

def write_bundler_table(tasks, token_file):
    output = ""
    output += """
    <table border="1" bordercolor="#000000" style="text-align:center;">
    <tr>
    <td>pipe_name</td>
    <td>index</td>
    <td>Creation Time</td>
    <td>Until Now (sec)</td>
    </tr>
    """
    time_now = int(os.path.getctime(token_file))
    tasks_simple = []
    for t in tasks:
        v = t["value"]
        pipe_name = str(v["bigpipe_state"]["pipe_name"]).strip('\"')
        pipelet_index = int(str(v["bigpipe_state"]["pipelet_index"]).strip('\"'))
        creation_time = int(t["attr"]["creation_time"]) / 1000000
        util_now = max(0, int(time_now - creation_time))
        task_simple = [pipe_name, pipelet_index, creation_time, util_now]
        tasks_simple.append(task_simple)
    tasks_sorted = sorted(tasks_simple, key=operator.itemgetter(0, 1))
        
    for t in tasks_sorted:
        #delay = int(time.time()) - int(v["state"]["start"]) - int(v["source"]["delay"])
        #tr_bgcolor = get_tr_bgcolor(max(0, delay / 60))
        tr_bgcolor = str(0)
        output += "<tr bgcolor=" + tr_bgcolor + ">\n"
        output += "<td>" + str(t[0]) + "</td>\n"
        output += "<td>" + str(t[1]) + "</td>\n"
        output += "<td>" + timestamp_datetime(t[2]) + "</td>\n"
        output += "<td>" + str(t[3]) + "</td>\n"
        output += "</tr>\n"
    output += """\n</table>\n"""
    return output

def write_html_file(output, html_filename):
    ofile = open(html_filename, 'w')
    ofile.write(output)
    ofile.close()
    return

def getTaskInfo(input="./fetcher.current.xml"):
    """
    get task info
    """
    tasklist = []
    getContent = lambda node: node.firstChild.nodeValue

    contents = file(input).read()
    contents = contents.decode('gbk').encode('utf-8')
    contents = contents.replace('gbk','utf-8')
    dom = parseString(contents)
    tasks = dom.getElementsByTagName("task")

    stack = []
    for task in tasks:
        t = {}
        for key in task.attributes.keys():
            t[key] = task.attributes[key].firstChild.nodeValue
        t["value"] = {}
        t["attr"] = {}
        t["attr"]["creation_time"] = task.getAttribute("creation_time")
        t["attr"]["eta"] = task.getAttribute("eta")
        t["attr"]["user_key"] = task.getAttribute("user_key")
        t["attr"]["owner"] = task.getAttribute("owner")

        content = getContent(task)
        content = content.replace(' ','')
        kv = content.split('\n')
        KVdict = t["value"]

        for item in kv :
            if item.endswith("{"):
                curSubKey = item.replace("{","")
                stack.append(KVdict)
                if curSubKey in KVdict.keys():
                    if not isinstance(KVdict[curSubKey],list):
                        tmp = KVdict[curSubKey]
                        KVdict[curSubKey]=[]
                        KVdict[curSubKey].append(tmp)
                    tmp_dict={}
                    KVdict[curSubKey].append(tmp_dict)
                    KVdict = tmp_dict
                else:
                    KVdict[curSubKey] = {}
                    KVdict = KVdict[curSubKey]
                continue
            elif item.startswith("}"):
                KVdict = stack.pop()
                continue
            ind = item.split(":")
            if len(ind)< 2 :
                continue
            if ind[0] in KVdict.keys():
                if not isinstance(KVdict[ind[0]],list):
                    tmp = KVdict[ind[0]]
                    KVdict[ind[0]]=[]
                    KVdict[ind[0]].append(tmp)
                KVdict[ind[0]].append(item[len(ind[0])+1:])
            else:
                KVdict[ind[0]] = item[len(ind[0])+1:]

        tasklist.append(t)
    return tasklist

def fetcher_task_filter(tasks, group_exclude):
    tasks_out = []
    for t in tasks:
        v = t["value"]
        # not show
        bool_exclude = str(v["name"]).strip('\"') in group_exclude
        # FAST_DIRECT_HDFS_FETCHER
        template_source = str(v["source"]["template_source"]).strip('\"')
        bool_fast_direct_hdfs_fetcher =  "YYYYmmdd" not in template_source and "YYYYMMDD" not in template_source
        if bool_exclude or bool_fast_direct_hdfs_fetcher:
            continue
        if t["attr"]["owner"] == "locker":
            continue
        tasks_out.append(t)

    return tasks_out

def timestamp_datetime(value):
    """
    print unix time
    """
    format = '%Y-%m-%d %H:%M:%S'
    value = time.localtime(int(value))
    dt = time.strftime(format, value)
    return dt

def datetime_timestamp(datetime_str):
    """
    translate datetime to unix time
    """
    d = datetime.datetime.strptime(datetime_str,"%Y-%m-%d %H:%M:%S")
    return int(time.mktime(d.timetuple()))

if __name__ == "__main__":
	################# for fetcher info ##############################
	output = ""

	tasks = getTaskInfo(monitor_path + fetcher_token_xml)
	tasks_less = fetcher_task_filter(tasks, [])
	output += write_page_header()
	#output += """<h2>FetcherTokenGroup(""" + str(len(tasks_less)) + """) [<a href=\"""" 
	#output += fetcher_output_html_more
	output += """<h2>FetcherTokenGroup(""" + str(len(tasks_less)) + """)</h2>"""
	output += write_fetcher_table(tasks_less, monitor_path + fetcher_token_xml);

	output += write_tail()
	write_html_file(output, monitor_path + fetcher_output_html);
	######################## for bundler info ############################
	tasks_asp = getTaskInfo(monitor_path + asp_bundler_token_xml)
	output = ""
	output += write_page_header()
	output += """<h2>AspBundlerTokenGroup(""" + str(len(tasks_asp)) + """)</h2>""" 
	output += write_bundler_table(tasks_asp, monitor_path + asp_bundler_token_xml);
	output += write_tail()
	write_html_file(output, monitor_path + asp_bundler_output_html);
