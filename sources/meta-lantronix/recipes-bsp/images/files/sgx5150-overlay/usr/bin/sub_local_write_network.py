import zmq
import time
import sys

#Subscribe and Get Data
context = zmq.Context()
sub=context.socket(zmq.SUB)
sub.setsockopt(zmq.SUBSCRIBE, b"")
sub.connect('tcp://localhost:5555')

port_num = sys.argv[1]
url="tcp://*:"+port_num

context2 = zmq.Context()
socket = context2.socket(zmq.PUB)
socket.bind(url)

while True:
        socket.send_string(sub.recv())
	#every 10 ms
        time.sleep(0.01)

