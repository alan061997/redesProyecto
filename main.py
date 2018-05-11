#!/usr/bin/python
# Librerias
from RFID import *
from BD import *

def main():
    srl = Sensor()
    srl.init_serial()
    print "Leyendo..."
