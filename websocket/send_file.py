import socket
import time

f = open("cat.gcode", 'r')

raw_data = f.read()
f.close()

#create an INET, STREAMing socket
s = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)
#now connect to the web server on port 80
# - the normal http port
s.connect(("localhost", 3000))

i = 0;
for line in raw_data.split("\n"):
	print("Line number: {}, data: {}".format(i, line));
	s.send(line);
	time.sleep(0.01);
	i += 1
s.close()
	#sock.send(line, 5000);
