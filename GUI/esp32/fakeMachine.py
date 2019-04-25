import threading
import socket
import time

class Pin():
    def __init__(self, pinId):
        pass
    
    
class PWM():
    pinId = None
    def __init__(self, pinId):
        self.pinId = pinId
    
    def duty(self, value):
        print("PWM pin id:", self.pinId, " PWM value:", value)


class Timer():
    period = 0
    callback = None
    
    def __init__(self, channel):
        pass
    
    def init(self, period, callback):
        self.period = period
        self.callback = callback
        print(self.callback)

        if (self.callback):        
            for i in range(100):
                self.callback(self)


    def deinit(self):
        pass

def restart():
    print("Restarting ... (not really)")



class UART():

    def __init__(self, channel, rx, tx):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect(("localhost", 3000)) # TODO move to a variable
        except:
            self.sock.connect(("localhost", 3001)) # TODO move to a variable

        self.len = 0
        self.data = b""
        
        
        self.lock = threading.Lock()
        self.thread1 = threading.Thread(target = self.commUpdateThread)#, args = self)
        self.thread1.start()

    def commUpdateThread(self):
        while(1):
            recvByte = self.sock.recv(1)
            
            self.lock.acquire()
            self.data += recvByte 
            self.lock.release()

            #print("Tralala")
            #print("Data: {}".format(self.data))
            #time.sleep(1)


    def write(self, rawData):
        #print("Sending data over serial. Data: {}".format(data))
        self.sock.send(rawData);
        #if (data):
        #    self.len = 3

    def read(self, numberOfBytes):
        #print("Reading data")
        retval = b""
        
        # mutex.acquire
        self.lock.acquire()
        
        if (numberOfBytes <= len(self.data)):
            retval = self.data[:numberOfBytes]
            self.data = self.data[numberOfBytes:]
        
        #mutex.release()
        self.lock.release()

        #print("Retval: {}".format(retval))
        return retval;
    
    def any(self):
        # mutex.acquire
        self.lock.acquire()
        
        result = len(self.data)
        
        #mutex.release()
        self.lock.release()

        return result
