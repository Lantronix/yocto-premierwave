import array
import json
import subprocess
import os
import os.path
import sys

flow_file = sys.argv[1]
os.chdir("/ltrx_user/")
file1 = os.path.basename(flow_file)
with open(flow_file, 'r') as fp:
    obj = json.load(fp)
    try:
        for i in range(0,len(obj['inputs'])):
            rec=obj['inputs'][i]
            file_name = rec['script path']
            script_path = "/ltrx_user/"+file_name
            if os.path.isfile(script_path):
                script_file = script_path
            else:
                script_file = "/usr/bin/"+file_name
            if os.path.isfile(script_file):
                params = str(rec['parameters'])
                cmd="PYTHONHOME=/usr/:/usr/ PYTHONPATH=/ltrx_user/python/lib/python2.7/site-packages/:/usr/lib/python2.7/site-packages/ PYTHONUNBUFFERED=y python "+script_file+" "+params+" &";
                print "function "+str(i)+"cmd: "+cmd
                os.system(cmd);
                print "started input function "+str(i)
            else:
                print "script ["+script_path+"] does not exist"
    except:
        print "Either inputs section not present or issue with format"
    try:
        for i in range(0,len(obj['functions'])):
            rec=obj['functions'][i]
            file_name = rec['script path']
            script_path = "/ltrx_user/"+file_name
            if os.path.isfile(script_path):
                script_file = script_path
            else:
                script_file = "/usr/bin/"+file_name
            if os.path.isfile(script_file):
                params = str(rec['parameters'])
                cmd="PYTHONHOME=/usr/:/usr/ PYTHONPATH=/ltrx_user/python/lib/python2.7/site-packages/:/usr/lib/python2.7/site-packages/ PYTHONUNBUFFERED=y python "+script_file+" "+params+" &";
                print "function "+str(i)+"cmd: "+cmd
                os.system(cmd);
                print "started input function "+str(i)
            else:
                print "script ["+script_path+"] does not exist"
    except:
        print "Either functions section not present or issue with format"
    try:
        for i in range(0,len(obj['outputs'])):
            rec=obj['outputs'][i]
            file_name = rec['script path']
            script_path = "/ltrx_user/"+file_name
            if os.path.isfile(script_path):
                script_file = script_path
            else:
                script_file = "/usr/bin/"+file_name
            if os.path.isfile(script_file):
                params = str(rec['parameters'])
                cmd="PYTHONHOME=/usr/:/usr/ PYTHONPATH=/ltrx_user/python/lib/python2.7/site-packages/:/usr/lib/python2.7/site-packages/ PYTHONUNBUFFERED=y python "+script_file+" "+params+" &";
                print "function "+str(i)+"cmd: "+cmd
                os.system(cmd);
                print "started output function "+str(i)
            else:
                print "script ["+script_path+"] does not exist"
    except:
        print "Either outputs section not present or issue with format"
