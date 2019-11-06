import zmq
import time
import serial
import sys
import threading

comPort = None
psocket = None
ssocket = None
SERIAL_THRESHOLD_MAX=2048

def check_url(url):
    if not "://" in url:
        return 0
    return 1

def RxThread():
    while True:
        msg = ssocket.recv()
        if msg and len(msg)>0:
            #print "tx:"+str(msg)
            comPort.write(msg)
            #print "tx-ed:"+str(msg)


def TxThread():
    while True:
        msg = comPort.read(size=SERIAL_THRESHOLD_MAX)
        if msg and len(msg)>0:
            #print "rx:"+str(msg)
            psocket.send(msg)

def main():

    global comPort
    global psocket
    global ssocket
    sub_path = ''
    serialport = ''
    baudrate = 115200
    bytesize = 8
    parity = 'N'
    stopbits = 1
    flowcontrol = "none"
    timeout = 0.2
    pub_path = ''

    length=len(sys.argv)

    i = 1 #0th parameter is file name
    while i < length:
        params = sys.argv[i].split('=', 1)
        if params[0] == "subscribe":
            res = check_url(params[1])
            if res==0:
                print "Invalid URL"
                return res
            sub_path = params[1]
        elif params[0] == "device":
            serialport = '/dev/tty'+params[1]
        elif params[0] == "baud":
            baudrate = int(params[1])
        elif params[0] == "dataBits":
            bytesize  = int(params[1])
        elif params[0] == "parity":
            parity = params[1]
        elif params[0] == "stopBits":
            stopbits = int(params[1])
        elif params[0] == "flowcontrol":
            flowcontrol = params[1]
        elif params[0] == "timeout":
            timeout = float(params[1])
        elif params[0] == "publish":
            res = check_url(params[1])
            if res==0:
                print "Invalid URL"
                return res
            pub_path =  params[1]
        else:
            print "Unknown paramter"
        i = i + 1

    if pub_path=='' or sub_path=='' or serialport=='':
        print "required subscriber, serialport and publish"
        return 0
    if flowcontrol.lower()!="none" and \
       flowcontrol.lower()!="hardware" and \
       flowcontrol.lower()!="software":
        print "provide valid flowcontrol"
        return 0
    if bytesize not in range (7,9):
        print "provide valid databits"
        return 0
    if stopbits not in range (1,3):
        print "provide valid stopbits"
        return 0
    if baudrate not in [300,600,1200,1800,2400,4800,7200,9600,14400,19200,38400,57600,115200,230400,460800,921600]:
        print "provide valid baudrate"
        return 0
    if parity not in ['N','O','E']: 
        print "provide valid parity"
        return 0

    while True:
        try:
            comPort = serial.Serial()
            comPort.port=serialport
            comPort.baudrate=baudrate
            comPort.bytesize=bytesize
            comPort.parity=parity
            comPort.stopbits=stopbits
            comPort.timeout=timeout
            if flowcontrol.lower()=='none':
                comPort.xonxoff=False
                comPort.rtscts=False
            if flowcontrol.lower()=='software':
                comPort.xonxoff=True
                comPort.rtscts=False
            if flowcontrol.lower()=='hardware':
                comPort.xonxoff=False
                comPort.rtscts=True
            comPort.open()
            break
        except serial.SerialException:
            print "Failed to open serial port"
            time.sleep(1)

    print "Opened serial port"
    pcontext = zmq.Context()
    psocket = pcontext.socket(zmq.PUB)
    psocket.bind(pub_path)

    scontext = zmq.Context()
    ssocket = scontext.socket(zmq.SUB)
    ssocket.setsockopt(zmq.SUBSCRIBE, b"")
    ssocket.connect(sub_path)

    serialRxThread = threading.Thread(target=RxThread)
    serialRxThread.start()
    serialTxThread = threading.Thread(target=TxThread)
    serialTxThread.start()

    serialRxThread.join()
    serialTxThread.join()

if __name__ == "__main__":
    main()
