import zmq
import time
import sys
import threading
import socket, select, sys

psocket=None
ssocket=None
s=None
NETWORK_THRESHOLD_MAX=2048
IsConnected=False;
connection=None

def check_ip(ip_addr):
    oct = ip_addr.split('.',3)
    if (len(oct) != 4):
        return 0
    tmp = int(oct[0])
    if tmp <=0 or tmp > 255:
        return 0
    i=1
    while i < 4:
        tmp = int(oct[i])
        print oct[i]
        if(tmp < 0 or tmp > 255):
            return 0
        else:
            return 1
    i=i+1

def check_protocol(proto):
    if(proto.lower() == "tcp"):
        return 1
    else:
        return 0

def check_url(url):
    if not "://" in url:
        return 0
    return 1

def NetworkTxThread(host, port, protocol):
    global IsConnected
    global connection
    print host, port, protocol
    ssocket.RCVTIMEO=1000
    print "TX thread started"
    while IsConnected:
        try:
            msg = ssocket.recv()
        except zmq.Again as e:
            continue
        if msg and len(msg):
            #print "tx:"+msg
            try:
                if protocol=="tcp":
                    connection.send(msg)
            except socket.error, msg:
                print "send error : %s" % msg
                time.sleep(1)

def NetworkRxThread(host, port, protocol):
    global IsConnected
    global connection
    print host, port, protocol
    while IsConnected:
        if protocol=="tcp":
            msg = connection.recv(NETWORK_THRESHOLD_MAX)
        if not msg:
            print "Disconnected"
            IsConnected=False;
            break
        if msg and len(msg):
            print "rx:"+msg
            psocket.send(msg)
def main():

    global psocket
    global ssocket
    global s
    global IsConnected
    global connection
    USING_PROTOCOL="tcp"

    length=len(sys.argv)
    if (length != 6):
        return 0

    i = 1 #0th parameter is file name
    while i < length:
        params = sys.argv[i].split('=',1)
        if params[0] == "subscribe":
            res = check_url(params[1])
            if res==0:
                print "Invalid URL"
                return res
            sub_path = params[1]
        elif params[0] == "ip":
            res = check_ip(params[1])
            if res==0:
                print "Invalid IP"
                return res
            REMOTE_HOST = params[1] #hostname or IP address
        elif params[0] == "port":
            try:
                REMOTE_PORT = int(params[1])#PORT
            except ValueError:
                print "Invalid port"
                return 0
        elif params[0] == "protocol":
            res = check_protocol(params[1])
            if res==0:
                print "Unsupported protocol"
                return res
            USING_PROTOCOL = params[1]#TCP, UDP
        elif params[0] == "publish":
            res = check_url(params[1])
            if res==0:
                print "Invalid URL"
                return res
            pub_path = params[1]
        else:
            print "Unknown parametrs"
        i=i+1

    while True:
        try:
            socket.inet_aton(REMOTE_HOST)
        except socket.error:
            # dns lookup
            REMOTE_HOST=socket.gethostbyname(REMOTE_HOST)

        if USING_PROTOCOL.lower()=="tcp":
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_address = (REMOTE_HOST, REMOTE_PORT)
            s.bind(server_address)
            s.listen(1)
            print "waiting for a connection"
            connection, client_address = s.accept()
            print "client connected:", client_address

        socket_list = [ s ]
        IsConnected = True

        pcontext = zmq.Context()

        psocket = pcontext.socket(zmq.PUB)
        psocket.bind(pub_path)

        scontext = zmq.Context()
        global ssocket
        ssocket = scontext.socket(zmq.SUB)
        ssocket.setsockopt(zmq.SUBSCRIBE, b"")
        ssocket.connect(sub_path)

        txThread = threading.Thread(target=NetworkTxThread, args=(REMOTE_HOST, REMOTE_PORT, USING_PROTOCOL.lower(),))
        txThread.start()
        rxThread = threading.Thread(target=NetworkRxThread, args=(REMOTE_HOST, REMOTE_PORT, USING_PROTOCOL.lower(),))
        rxThread.start()

        txThread.join()
        rxThread.join()

        try:
            s.shutdown(2)
        except socket.error, msg:
            print "Socket shutdown Error : %s" % msg
	connection.close()
        s.close()

        psocket.close()
        ssocket.close()


if __name__ == "__main__":
        main()
