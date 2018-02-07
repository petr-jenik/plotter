'''
The MIT License (MIT)
Copyright (c) 2013 Dave P.
'''

import signal
import sys
import ssl
from SimpleWebSocketServer import WebSocket, SimpleWebSocketServer, SimpleSSLWebSocketServer
from optparse import OptionParser

import socket

#create an INET, STREAMing socket
s = socket.socket(
socket.AF_INET, socket.SOCK_STREAM)
#now connect to the web server on port 80
# - the normal http port
s.connect(("localhost", 3000))


class SimpleEcho(WebSocket):

   def handleMessage(self):
      self.sendMessage(self.data)

   def handleConnected(self):
      pass

   def handleClose(self):
      pass

COMMAND_CALIBRATE = "COMMAND_CALIBRATE:";
COMMAND_GO_TO_LEFT = "COMMAND_GO_TO_LEFT:";
COMMAND_GO_TO_RIGHT = "COMMAND_GO_TO_RIGHT:";
COMMAND_SETTING = "COMMAND_SETTING:";
COMMAND_GCODE = "GCODE:";

RESULT_OK = u"RESULT_OK";
RESULT_NOT_OK = u"RESULT_NOT_OK";


commands = {COMMAND_GCODE, COMMAND_CALIBRATE, COMMAND_GO_TO_LEFT, COMMAND_GO_TO_RIGHT, COMMAND_SETTING}

clients = []
class SimpleChat(WebSocket):

	def processData(self, message):
		print("received data:" + str(message))

		for cmd in commands:
			if message.startswith(cmd):
				print("Valid command received");

				if message.startswith(COMMAND_GCODE):
						#gcode_line = message[len(COMMAND_GCODE):-1].strip() + "\n"
						print(gcode_line)
						s.send(gcode_line)
				self.sendMessage(RESULT_OK);
				return;
		self.sendMessage(RESULT_NOT_OK);

	def sendToAll(self, message):
		for client in clients:
			client.sendMessage(message)
		print("Sending message...")

	def handleMessage(self):
		print("Number of clients: {}".format(len(clients)))
		#print(self.data)
		x = self.data
		#print("x", x)
		self.processData(x)
		self.data = ""

	def handleConnected(self):
		print (self.address, 'connected')
		for client in clients:
			client.sendMessage(self.address[0] + u' - connected')
		clients.append(self)

	def handleClose(self):
		clients.remove(self)
		print (self.address, 'closed')
		for client in clients:
			client.sendMessage(self.address[0] + u' - disconnected')


if __name__ == "__main__":

   parser = OptionParser(usage="usage: %prog [options]", version="%prog 1.0")
   parser.add_option("--host", default='', type='string', action="store", dest="host", help="hostname (localhost)")
   parser.add_option("--port", default=8000, type='int', action="store", dest="port", help="port (8000)")
   parser.add_option("--example", default='echo', type='string', action="store", dest="example", help="echo, chat")
   parser.add_option("--ssl", default=0, type='int', action="store", dest="ssl", help="ssl (1: on, 0: off (default))")
   parser.add_option("--cert", default='./cert.pem', type='string', action="store", dest="cert", help="cert (./cert.pem)")
   parser.add_option("--key", default='./key.pem', type='string', action="store", dest="key", help="key (./key.pem)")
   parser.add_option("--ver", default=ssl.PROTOCOL_TLSv1, type=int, action="store", dest="ver", help="ssl version")

   (options, args) = parser.parse_args()

   cls = SimpleChat
   if options.example == 'chat':
      cls = SimpleChat

   if options.ssl == 1:
      server = SimpleSSLWebSocketServer(options.host, options.port, cls, options.cert, options.key, version=options.ver)
   else:
      server = SimpleWebSocketServer(options.host, options.port, cls)

   def close_sig_handler(signal, frame):
      server.close()
      sys.exit()

   signal.signal(signal.SIGINT, close_sig_handler)

   server.serveforever()
