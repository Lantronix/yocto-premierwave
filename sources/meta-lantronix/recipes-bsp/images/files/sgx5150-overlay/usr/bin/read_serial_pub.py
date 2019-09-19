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
	socket = context.socket(zmq.PUB)
	socket.bind("tcp://127.0.0.1:5555")
        ser = serial.Serial(SERIAL_PORT, SERIAL_RATE)
        while True:
                reading = ser.readline().decode('utf-8')
		socket.send_string(reading)

if __name__ == "__main__":
        main()

