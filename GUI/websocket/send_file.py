import socket
import time

import sys

if (len(sys.argv) > 1):
	filename = sys.argv[1]	
else:
	filename = "cat.gcode"

f = open(filename, 'r')

raw_data = f.read()
f.close()

#create an INET, STREAMing socket
s = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)
#now connect to the web server on port 80
# - the normal http port
s.connect(("localhost", 3000))

i = 0
numberOfSentBytes = 0

for line in raw_data.split("\n"):

	data = line + '\n'
	print("Line number: {}, data: {}".format(i, data))
	numberOfSentBytes += len(data)
	s.send(data);
	#response = s.recv(100)
	#print("Response {}".format(response))
	#time.sleep(0.05);
	i += 1

s.close()
print("numberOfSentBytes: {}".format(numberOfSentBytes))
	#sock.send(line, 5000);