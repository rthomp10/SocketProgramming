#!/usr/bin/python3
#
# How to use:
#   1. After starting the server, start the client with:
#      ./client-simple.py [server-name/address] [port] [data]
#
# Notes:
#   1. This uses Python3, so there are some differences vs Python2
#      a) string handling
#         - Python2 strings are default "simple ASCII"
#         - Python3 strings are default "unicode"
#      b) input/output functions
#         - Python2 uses print ".." and raw_input(..)
#         - Python3 uses print(..) and input(..)
#

import socket
import sys

# Get server name and bind to port
s_host = '127.0.1.1'
s_port = 12345

if len(sys.argv) == 2:
    s_host = sys.argv[1]
if len(sys.argv) == 3:
    s_port = sys.argv[2]
    
s_ip = socket.gethostbyname(s_host)
s_address = (s_ip, int(s_port))

# Create TCP socket and connect to server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(s_address)

print('\n(client) Connected to:')
print('\t host:', s_host)
print('\t ip/port:', s_address)

# Get data from user
local_string = input('\n\t Enter some data to send: ')

# Send some data, first converting to bytes
sent_data = local_string.encode('UTF-8')
s.send(sent_data)

print('\n\t Sent:')
print('\t bytes:', sent_data)
print('\t unicode:', sent_data.decode('UTF-8'))

# Receive some data from the server
rec_data = s.recv(1024)

print('\n\t Received:')
print('\t bytes:', rec_data)
print('\t unicode:', rec_data.decode('UTF-8'))

# Close the connection
print('\n(client) Exiting...\n')
s.close()
sys.exit()
