import zmq, time, sys
import syslog, threading
import socket, select, sys
import syslog, subprocess
from collections import deque

psocket=None
ssocket=None
s=None
NETWORK_THRESHOLD_MAX=2048
IsConnected=False;
connection=None
data=deque([])
bufferqlen=0
MAXBUFFLEN=0

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
    global bufferqlen
    syslog.syslog(syslog.LOG_DEBUG, "network tx: "+host+":"+str(port)+","+protocol)
    ssocket.RCVTIMEO=1000
    while IsConnected:
        while data:
            msg = data.popleft()
            if msg and len(msg):
                try:
                    sent=connection.send(msg)
                    bufferqlen = bufferqlen - sent;
                except socket.error as e:
                    syslog.syslog(syslog.LOG_ERR, "send error : %s" % e)
                    bufferqlen = bufferqlen - sent;
                    if sent != len(msg):
                        unsentmsg = msg[sent:]
                        data.insert(0, unsentmsg)
                    time.sleep(1)
        try:
            msg = ssocket.recv()
        except zmq.Again as e:
            continue
        if msg and len(msg):
            #syslog.syslog(syslog.LOG_DEBUG, "tx:"+msg)
            try:
                if protocol=="tcp":
                    connection.send(msg)
            except socket.error as msg:
                syslog.syslog(syslog.LOG_ERR, "send error : %s" % msg)
                if sent != len(msg):
                    unsentmsg = msg[sent:]
                    data.append(unsentmsg)
                    bufferqlen = bufferqlen + len(unsentmsg)
                time.sleep(1)

def NetworkRxThread(host, port, protocol):
    global IsConnected
    global connection
    syslog.syslog(syslog.LOG_DEBUG, "network rx: "+host+":"+str(port)+","+protocol)
    while IsConnected:
        if protocol=="tcp":
            msg = connection.recv(NETWORK_THRESHOLD_MAX)
        if not msg:
            syslog.syslog(syslog.LOG_WARNING, "Disconnected")
            IsConnected=False;
            break
        if msg and len(msg):
            psocket.send(msg)
def main():

    global psocket
    global ssocket
    global s
    global IsConnected
    global connection
    global bufferqlen
    global MAXBUFFLEN
    USING_PROTOCOL="tcp"

    cmd="free -m | grep Mem: | awk '{print $4}'"
    val=subprocess.Popen(cmd,stdin=subprocess.PIPE,stdout=subprocess.PIPE, shell=True).stdout.read()
    freemem=int(val.strip())

    length=len(sys.argv)
    if (length != 7):
        syslog.syslog(syslog.LOG_ERR, "Invalid parameters")
        return 0

    i = 1 #0th parameter is file name
    while i < length:
        params = sys.argv[i].split('=',1)
        if params[0] == "subscribe":
            res = check_url(params[1])
            if res==0:
                syslog.syslog(syslog.LOG_ERR, "Invalid subscribe URL")
                return res
            sub_path = params[1]
        elif params[0] == "ip":
            res = check_ip(params[1])
            if res==0:
                syslog.syslog(syslog.LOG_ERR, "Invalid server IP")
                return res
            REMOTE_HOST = params[1] #hostname or IP address
        elif params[0] == "port":
            try:
                REMOTE_PORT = int(params[1])#PORT
            except ValueError:
                syslog.syslog(syslog.LOG_ERR, "Invalid port")
                return 0
        elif params[0] == "protocol":
            res = check_protocol(params[1])
            if res==0:
                syslog.syslog(syslog.LOG_ERR, "Unsupported protocol")
                return res
            USING_PROTOCOL = params[1]#TCP, UDP
        elif params[0] == "buffersize":
            req_buff = int(params[1])
            if req_buff < 0 and req_buff > 2:
                syslog.syslog(syslog.LOG_ERR, "Invalid buffer size. Supported values 0..2")
                return 0
            if req_buff <= (freemem-2):
                MAXBUFFLEN = req_buff*1024*1024
            else:
                syslog.syslog(syslog.LOG_WARNING, "Requested memory not available. Buffering is not enabled")
        elif params[0] == "publish":
            res = check_url(params[1])
            if res==0:
                syslog.syslog(syslog.LOG_ERR, "Invalid publish URL")
                return res
            pub_path = params[1]
        else:
            syslog.syslog(syslog.LOG_WARNING, "Unknown parametrs")
        i=i+1

    while True:
        try:
            socket.inet_aton(REMOTE_HOST)
        except socket.error:
            # dns lookup
            REMOTE_HOST=socket.gethostbyname(REMOTE_HOST)

        scontext = zmq.Context()
        global ssocket
        ssocket = scontext.socket(zmq.SUB)
        ssocket.setsockopt(zmq.SUBSCRIBE, b"")
        ssocket.connect(sub_path)

        if USING_PROTOCOL.lower()=="tcp":
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server_address = (REMOTE_HOST, REMOTE_PORT)
            s.bind(server_address)
            s.listen(1)
            syslog.syslog(syslog.LOG_DEBUG, "waiting for a connection")
            s.settimeout(1)
            while True:
                try:
                    connection, client_address = s.accept()
                    syslog.syslog(syslog.LOG_DEBUG, "client connected:"+ str(client_address))
                    break
                except socket.timeout as e:
                    try:
                        msg = ssocket.recv(1) 
                        try:
                            if MAXBUFFLEN and msg and len(msg)>0:
                                if bufferqlen+len(msg) < MAXBUFFLEN:
                                    bufferqlen = bufferqlen + len(msg)
                                    data.append(msg)
                        except:
                            continue
                    except zmq.Again as e:
                        continue

        socket_list = [ s ]
        IsConnected = True

        pcontext = zmq.Context()

        psocket = pcontext.socket(zmq.PUB)
        psocket.bind(pub_path)


        txThread = threading.Thread(target=NetworkTxThread, args=(REMOTE_HOST, REMOTE_PORT, USING_PROTOCOL.lower(),))
        txThread.start()
        rxThread = threading.Thread(target=NetworkRxThread, args=(REMOTE_HOST, REMOTE_PORT, USING_PROTOCOL.lower(),))
        rxThread.start()

        txThread.join()
        rxThread.join()

        try:
            s.shutdown(2)
        except socket.error as msg:
            syslog.syslog(syslog.LOG_ERR, "Socket shutdown Error : %s" % msg)
        connection.close()
        s.close()

        psocket.close()
        ssocket.close()


if __name__ == "__main__":
        main()
