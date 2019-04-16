
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
        pass
        self.len = 0

    def write(self, data):
        print("Sending data over serial. Data: {}".format(data))
        if (data):
            self.len = 3
        
    def read(self, numberOfBytes):
        if (self.len != 0):
            self.len = 0
            return b"ACK"
    
    def any(self):
        return self.len
