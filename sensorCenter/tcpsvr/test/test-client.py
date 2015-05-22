from socket import *
import time

Host = "10.140.28.147"
Port = 45000
Addr = (Host,Port)

def makeClient():
    s = socket(AF_INET,SOCK_STREAM);
    s.connect(Addr);
    count = 0;
    while (count < 10 ):
        data = "hello Server "+str(count)
        count += 1
        res = s.send(data)
        print str(count)+":" + str(res)
        time.sleep(2)
    s.close();


if __name__ == "__main__":
    makeClient()
