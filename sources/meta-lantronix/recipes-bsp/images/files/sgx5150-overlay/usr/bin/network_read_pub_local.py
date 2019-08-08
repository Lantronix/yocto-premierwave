import zmq
import time
import sys

ip_add = sys.argv[1]
port_num = sys.argv[2]

#Subscribe and Get Data
context = zmq.Context()
sub=context.socket(zmq.SUB)
sub.setsockopt(zmq.SUBSCRIBE, b"")
url="tcp://"+ip_add+":"+port_num
sub.connect(url)

context2 = zmq.Context()
socket2 = context2.socket(zmq.PUB)
socket2.bind("tcp://127.0.0.1:5555")

while True:
        socket2.send_string(sub.recv())
	#every 10 ms
        time.sleep(0.01)
