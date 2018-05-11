#!/usr/bin/python
# Librerias
import serial
import serial.tools.list_ports as listPorts
import time


# Clase que se encarga de lo que corresponde a recibir datos del serial.
class RFID():
    # Contructor de la clase RFID
    def __init__(self, *args, **kwargs):
        # Inicializacion de una variable para la entrada serial del RFID en valor Null
        self.serial = None

    # Metodo para la delimitacion del puerto del serial
    def port_names(self):
        # Inicializacion de la lista pnames que guardara el nombre del puerto.
        pnames = []
        # Ciclo de busqueda por cada puerto en la lista de puertos
        for port in listPorts.comports():
            # Si el primer puerto es igual a "ttyAMA" continua con el codigo
            if "ttyAMA" in port[0]:
                continue
            # De lo contrario se agrega a la lista de pnames
            else:
                pnames.append(port[0])
        # Regresa el puerto
        return pnames[0]

    # Metodo de inicializacion del serial en python
    def init_serial(self):
        # Creamos la variable que va a escuchar por la frecuencia 9600 del serial.
        self.serial = serial.Serial(self.port_names(), 9600, timeout=1)

    # Metodo de obtencion de clave RFID a traves del puerto serial
    def get_id(self):
        # Inicializacion de la variable id para guardar las claves recibidas del serial
        id = ''
        # Ciclo que recibe los seriales en la linea de espera
        while (self.serial.inWaiting() > 0):
            # Parsea la linea del serial sin espacios en blanco
            id = self.serial.readline().decode('utf-8').strip()
        # Parsea la linea del serial sin espacios en blanco
        id = self.serial.readline().decode('utf-8').strip()
        # Regresa la clave
        return id


def main():
    ins = RFID()
    ins.init_serial()
    print("Iniciando Serial:\n {}".format(ins.serial))
    time.sleep(5)
    while True:
        print("'{}'".format(ins.get_id()))


if __name__ == '__main__':
    main()
