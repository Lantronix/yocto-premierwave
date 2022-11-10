import array, json, os
import subprocess, sys
import os.path
import re, time
import syslog

flow_file = sys.argv[1]
os.chdir("/ltrx_user/")
file1 = os.path.basename(flow_file)
file_name = "/tmp/"+sys.argv[1]+".txt"
file1 = open(file_name,"a")
file1.truncate(0)

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
                cmd=". /etc/pythonenv; python "+script_file+" "+params+" &";
                syslog.syslog(syslog.LOG_DEBUG, "function "+str(i)+"cmd: "+cmd)
                os.system(cmd)
                time.sleep(1)
                s_string = script_file+" "+params
                cmd1 = "ps -ef | grep '{0}' | grep -v grep".format(s_string)
                out = subprocess.getoutput(cmd1)
                file1.writelines(str(out)+'\n')
                syslog.syslog(syslog.LOG_DEBUG, "started input function "+str(i))
            else:
                syslog.syslog(syslog.LOG_WARNING, "script ["+script_path+"] does not exist")
    except:
        syslog.syslog(syslog.LOG_ERR,"Either inputs section not present or issue with format")
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
                cmd=". /etc/pythonenv; python "+script_file+" "+params+" &";
                syslog.syslog(syslog.LOG_DEBUG, "function "+str(i)+"cmd: "+cmd)
                os.system(cmd);
                time.sleep(1)
                s_string = script_file+" "+params
                cmd1 = "ps -ef | grep '{0}' | grep -v grep".format(s_string)
                out = subprocess.getoutput(cmd1)
                file1.writelines(str(out)+'\n')
                syslog.syslog(syslog.LOG_DEBUG, "started input function "+str(i))
            else:
                syslog.syslog(syslog.LOG_WARNING, "script ["+script_path+"] does not exist")
    except:
        syslog.syslog(syslog.LOG_ERR, "Either functions section not present or issue with format")
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
                cmd=". /etc/pythonenv; python "+script_file+" "+params+" &";
                syslog.syslog(syslog.LOG_DEBUG,"function "+str(i)+"cmd: "+cmd)
                os.system(cmd);
                time.sleep(1)
                s_string = script_file+" "+params
                cmd1 = "ps -ef | grep '{0}' | grep -v grep".format(s_string)
                out = subprocess.getoutput(cmd1)
                file1.writelines(str(out)+'\n')
                syslog.syslog(syslog.LOG_DEBUG, "started output function "+str(i))
            else:
                syslog.syslog(syslog.LOG_WARNING, "script ["+script_path+"] does not exist")
    except:
        syslog.syslog(syslog.LOG_ERR, "Either outputs section not present or issue with format")
    file1.close()
