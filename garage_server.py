#!/usr/bin/python

# Basic server to listen for messages from arduino garage sensor.

import socket, logging, smtplib, datetime

HOST = '127.0.0.1'  # use public IP address here.
PORT = 51729

smtp_server = 'localhost'
emails = ['sussman@red-bean.com', 'eff@red-bean.com']

logging.basicConfig(filename='garage.log',
                    level=logging.DEBUG,
                    format='%(asctime)s %(message)s',
                    datefmt='%m/%d/%Y %I:%M:%S %p :  ')


def startSocket():
  # Wait for connection
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  print 'Listening for incoming connection on port ' + str(PORT)
  s.bind((HOST, PORT))
  s.listen(1)
  conn, addr = s.accept()
  print 'Incoming connection from: ', addr
  return conn

# Receive data forever, until client disconnects.
def receiveLoop(conn):
  while 1:
    data = conn.recv(1024)
    if not data:
      break
    onMessage(data)
  conn.close() 

# Log the data, possibly alert someone.
def onMessage(data):
  msg = data.strip()
  print 'Data received.'
  now = datetime.datetime.now()
  if msg is 'O':
    logging.info('Garage switch is OPEN')
    send_email('Garage door opened: ' + now.strftime("%Y-%m-%d %H:%M"))
  elif msg == 'C':
    logging.info('Garage switch is CLOSED')
    send_email('Garage door closed: ' + now.strftime("%Y-%m-%d %H:%M"))
  else:
    logging.info('Unknown message received: ' + msg)


def send_email(msg):
  logging.debug('Sending emails.')
  for address in emails:
    #print 'I YAM SENDING MAIL TO ' + address + ': ' + msg
    #server = smtplib.SMTP(smtp_server)
    #server.sendmail("sussman@red-bean.com", address, msg)
    #server.quit()


while 1:
  connection = startSocket()
  receiveLoop(connection)
