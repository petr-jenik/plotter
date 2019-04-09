#import socket
import time

import serial
import sys

if (len(sys.argv) > 1):
	filename = sys.argv[1]	
else:
	filename = "cat.gcode"

f = open(filename, 'r')

raw_data = f.read()
f.close()

#create an INET, STREAMing socket
#s = socket.socket(
#    socket.AF_INET, socket.SOCK_STREAM)
#now connect to the web server on port 80
# - the normal http port
#s.connect(("localhost", 3000))

port = serial.Serial("/dev/ttyUSB0", 115200);

i = 0
numberOfSentBytes = 0

for line in raw_data.split("\n"):

	data = line + '\n'
	print("Line number: {}, data: {}".format(i, data))
	numberOfSentBytes += len(data)
	port.write(data)

	time.sleep(0.01)

        startTime = time.time()
        
        response = b""
        
        while(1):
            numberOfRxBytes = port.in_waiting
            response += port.read(numberOfRxBytes)

            if response != "":
                print("Repsonse: {}".format(response))

            if "ACK" in response:
                break;
        
            if (time.time() - startTime) > 0.1:
                print("Timeout - Send command again")
                startTime = time.time()
                port.write(data)

	#s.send(data);
	#response = s.recv(100)
	#print("Response {}".format(response))
	#time.sleep(0.05);
	i += 1

#s.close()
print("numberOfSentBytes: {}".format(numberOfSentBytes))
	#sock.send(line, 5000);
