from microWebSrv import MicroWebSrv
import json
import math
import time


try:
    import machine
except:
    import fakeMachine as machine

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------


### Timer is required for flashing (pulsing) with LEDs

timer = machine.Timer(0)  # Instatiate hardware timer

timerEnabled = False

def enableTimer():
    global timerEnabled
    
    if timerEnabled == False:
        print("Timer enabled")
        timer.init(period=TIMER_PERIOD_MS, callback=timerCallback)  # Init and start timer
        timerEnabled = True

def disableTimer():
    global timerEnabled
    
    if timerEnabled:
        print("Timer disabled")
        timer.deinit()  # Dispose of timer
        timerEnabled = False

t = 0 # Time in ms

def timerCallback(timer):
    global t
    t = (t + TIMER_PERIOD_MS) % 60000 # Time in miliseconds in range from 0s upto 1 minute.
    for light in lights:
        light.pulseUpdate(t);

#######
# Websocket callbacks
#######

def _acceptWebSocketCallback(webSocket, httpClient) :
    print("WS ACCEPT")
    webSocket.RecvTextCallback   = _recvTextCallback
    webSocket.RecvBinaryCallback = _recvBinaryCallback
    webSocket.ClosedCallback     = _closedCallback

    #message = getInitialMessage()
    #webSocket.SendText("Plotter connected")
    #clients.append(webSocket)
    #print("Number of clients: {}".format(len(clients)))


def _recvTextCallback(webSocket, msg) :
        print("WS RECV TEXT : %s" % msg)

        #global pwmValue
        #print("Received message: %s" % msg)
        #print(type(msg))
        #print(len(msg))
        
        sendLineToPlotter(webSocket, msg)
        #parseMessage(webSocket, msg)

        #print("RX - PWM value:", pwmValue)
        #webSocket.SendText("Reply for %s" % msg)

def _recvBinaryCallback(webSocket, data) :
	print("WS RECV DATA : %s" % data)

def _closedCallback(webSocket) :
        print("WS CLOSED")
        #global pwmValue

        #clients.remove(webSocket)

        #print("Close - PWM value:", pwmValue)
        #print("Number of clients: {}".format(len(clients)))

# ----------------------------------------------------------------------------

#routeHandlers = [
#	( "/test",	"GET",	_httpHandlerTestGet ),
#	( "/test",	"POST",	_httpHandlerTestPost )
#]

gUart = None

def sendLineToPlotter(webSocket, line):
    global gUart
    #port = serial.Serial("/dev/ttyUSB0", 115200);

    gUart.write(line)
    time.sleep(0.01)

    startTime = time.time()
    response = b""

    try:
        while(1):
            numberOfRxBytes = gUart.any()
            response += gUart.read(numberOfRxBytes)

            if response != "":
                print("Repsonse: {}".format(response))

            if b"ACK" in response:
                break;
        
            if (time.time() - startTime) > 0.1:
                print("Timeout - Send command again")
                startTime = time.time()
                gUart.write(line)

            #print("Response {}".format(response))
            #time.sleep(0.05);  
        webSocket.SendText("OK")
    except Exception as e:
        webSocket.SendText(str(e))


def start():    
    global gUart
    gUart = machine.UART(1, rx=14, tx=12)

    srv = MicroWebSrv(webPath='www/')

    #srv = MicroWebSrv(routeHandlers=routeHandlers, webPath='/www/')
    srv.MaxWebSocketRecvLen     = 256
    srv.WebSocketThreaded		= True #False
    srv.AcceptWebSocketCallback = _acceptWebSocketCallback
    srv.Start()

    # ----------------------------------------------------------------------------


if __name__ == "__main__":
        
    if 0:
        gUart = machine.UART(1, rx=14, tx=12)
        sendLineToPlotter(None, b"G0 Z42")
    else:
        start()



