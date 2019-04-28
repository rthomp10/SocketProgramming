#!/usr/bin/python3
#
# How to use:
#   1. Start the server with: ./server-simple.py
#   2. Use the server's name/address/port in the client's command-line
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

# Get localhost name
s_host = socket.gethostname()
s_fqdn = socket.getfqdn()
s_ip = socket.gethostbyname(s_host)
s_port = 12345
s_address = (s_ip, s_port)

# Create TCP socket, and set the SO_REUSEADDR so if the
# server re-starts quickly, the automatic TCP timeout
# (TIME_WAIT) of ~2 minutes doesn't apply.

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind socket to local port, and start listening
s.bind(s_address)
s.listen()

while True:

    print('\n(server) Listening on:')
    print('\t host:', s_host)
    print('\t fqdn:', s_fqdn)
    print('\t ip/port:', s_address)

    # Accept a connection from a client
    conn, addr = s.accept()

    print('\n\t Connection from:')
    print('\t peername:', conn.getpeername())
    print('\t address:', addr)

    # Receive ASCII data from the connected socket
    rec_data = conn.recv(1024)
    if not rec_data: break

    print('\n\t Received:')
    print('\t bytes:', rec_data)
    print('\t unicode:', rec_data.decode('UTF-8'))
    
    # Convert the data from ASCII to unicode string
    # and munge it before sending back to client
    dum = rec_data.decode('UTF-8')
    dum = 'REVERSE MESSAGE: ' + dum[::-1]

    # Convert the data from unicode string to ASCII
    # and send back to the client
    print('\n\t Sending back to client:')
    print('\t bytes:', dum.encode('UTF-8'))
    print('\t unicode:', dum)
    munge_data = dum.encode('UTF-8')
    conn.send(munge_data)

    # Close the connection
    conn.close()

    # Check for special command (convert to unicode)
    if rec_data.decode('UTF-8') == 'BYE':
        break
    
print('\n(server) Exiting ...')
s.close()
sys.exit()
        
