import array
import json
import subprocess
import os
import os.path
import sys

flow_file = sys.argv[1]

file1 = os.path.basename(flow_file)
pid_file = "/tmp/"+file1+"_pid.txt"
f = open(pid_file,"w+")
with open(flow_file, 'r') as fp:
    obj = json.load(fp)
for i in range(0,len(obj)):
	file_name = obj[i]['script_path']
        script_path = "/ltrx_user/"+file_name 
        if os.path.isfile(script_path):
            script_file = script_path
        else:
            script_file = "/usr/bin/"+file_name

	temp = str(obj[i]['parameters'])
	params = temp.split(',')
	params_new = []
	params_new.append("python")
	params_new.append(str(script_file))
	for i in range(0,len(params)):
		params_new.append((params.pop(0)))
	print("Command Executing", params_new)
	returned_output = subprocess.Popen(params_new)
	print("Completed Launching "+script_file)
	print("Process ID", returned_output.pid)
	f.write(str(returned_output.pid))
	f.write("\n")
f.close()
