#!/usr/bin/python
# Librerias
from RFID import *
from BD import *

def main():
    ins = RFID()
    data = BD()
    ins.init_serial()
    print("Iniciando Serial:\n {}".format(ins.serial))
    time.sleep(5)
    while True:
        data_serial = ins.get_id()
        print("'{}'".format(data_serial))
        data.insert_data(data_serial)


if __name__ == '__main__':
    main()
