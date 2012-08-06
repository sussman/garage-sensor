#!/usr/bin/env python

# Basic server to listen for messages from arduino garage sensor.

import socket

HOST = '10.0.1.144'
PORT = 51729

# Wait for connection
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Listening for incoming connection on port ' + str(PORT)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print 'Incoming connection from: ', addr

# Receive data forever, until client disconnects.
def receiveLoop():
  while 1:
    data = conn.recv(1024)
    if not data:
      break
    onMessage(data)
  conn.close() 

# Log the data, possibly alert someone.
def onMessage(data):
  print "Got data: " + data

receiveLoop();
