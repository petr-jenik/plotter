import socket
import time

import sys

if (len(sys.argv) > 1):
	filename = sys.argv[1]	
else:
	filename = "logo.gcode"

f = open(filename, 'r')

raw_data = f.read()
f.close()

#create an INET, STREAMing socket
s1 = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)
s2 = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)

#now connect to the web server on port 80
# - the normal http port
s1.connect(("localhost", 3000))
s2.connect(("localhost", 3001))


i = 0
numberOfSentBytes = 0

for line in raw_data.split("\n"):

	data = line + '\n'
	print("Line number: {}, data: {}".format(i, data))
	numberOfSentBytes += len(data)
	s1.send(data);
	s2.send(data);
	#response = s.recv(100)
	#print("Response {}".format(response))
	#time.sleep(0.05);
	i += 1

s1.close()
s2.close()
print("numberOfSentBytes: {}".format(numberOfSentBytes))
	#sock.send(line, 5000);
