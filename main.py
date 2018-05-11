#!/usr/bin/python
# Librerias
from RFID import *
from BD import *

def main():
    srl = RFID()
    srl.init_serial()
    print "Leyendo...\n"
    time.sleep(3)

    while True:
        time.sleep(2)
        id = srl.get_id()
        print "\nID: " + id

        time.sleep(2)
        print "\nRegistrando..."
        bd = BD()
        bd.insert_data(id)
        time.sleep(2)

if __name__ == '__main__':
    main()
