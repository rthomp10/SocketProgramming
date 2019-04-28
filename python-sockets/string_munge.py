#!/usr/bin/python3
#
# Some useful string munging techniques with Python
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

import sys

# Parse command-line arguments
if len(sys.argv) == 2:
    s_host = sys.argv[1]
if len(sys.argv) == 3:
    s_port = sys.argv[2]
    
# Get text from user
dum_unicode = input('\nEnter some text with several characters: ')

# Convert from unicode string to ASCII
dum_ascii = dum_unicode.encode('UTF-8')
dum_reversed = dum_ascii[::-1]

print('\t Unicode (str):\t', dum_unicode)
print('\t String (flip):\t', dum_unicode[::-1])
print('\t 2nd char:\t', dum_unicode[1])
print('\t last char:\t', dum_unicode[-1])
print('\t ASCII (bytes):\t', dum_ascii)
print('\t ASCII (flip):\t',dum_reversed)

# Get number from user
dum = input('\nEnter a number: ')
if not dum.isdigit():
    print('Not a number')
    sys.exit()

dum_integer = int(dum,10)
dum_bin = bin(dum_integer)
dum_hex = hex(dum_integer)
print('\t Integer: \t', dum_integer)
print('\t Binary:  \t', dum_bin)
print('\t Hex: \t\t', dum_hex)

# IP numbers
dum = input('\nEnter an IP number (x.y.z.w): ')
dumv = dum.split('.')

if len(dumv) != 4:
    print('Wrong IP format')
    sys.exit()

for i in range(4):
    dd = dumv[i]
    if (not dd.isdigit()) or (int(dd) < 0) or (int(dd) > 255):
        print('Element {} ({}) is improper octet: '.format(i,dd))
        sys.exit()

print(' ')
print(f'1) forward, column, center\t [{dum:^20}]')
print(f'2) reverse, column, right\t [{dum[::-1]:>20}]')
print(f'3) reverse, column, left\t [{dum[::-1]:<20}]')
print('-' *60)
print('4) positional, names\t first: [{a:^5}] third: [{b:^5}]'.format(b=dumv[2],a=dumv[0]))
print('5) positional, list \t [{:<5}].[{:^5}].[{:^5}].[{:>5}]'.format(*dumv))
print('-' *60)
print('6) format, list, zeros\t {:03d}.{:03d}.{:03d}.{:03d}'.format(int(dumv[0]),int(dumv[1]),int(dumv[2]),int(dumv[3])))
print('7) format, list, map\t {:03d}.{:03d}.{:03d}.{:03d}'.format(*list(map(int,dumv))))



