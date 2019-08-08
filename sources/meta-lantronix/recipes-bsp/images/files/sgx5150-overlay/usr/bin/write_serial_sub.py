import zmq
import time
import serial
import sys

serial_port = '/dev/tty'+sys.argv[1]
baud_rate = sys.argv[2]

SERIAL_PORT = serial_port
SERIAL_RATE = baud_rate

def main():
	context = zmq.Context()
	socket = context.socket(zmq.SUB)
	socket.setsockopt(zmq.SUBSCRIBE, b"")
	socket.connect("tcp://127.0.0.1:5555")
        ser = serial.Serial(SERIAL_PORT, SERIAL_RATE)
        while True:
                ser.write(socket.recv())
if __name__ == "__main__":
        main()
