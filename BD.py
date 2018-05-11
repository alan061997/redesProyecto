#!/usr/bin/python
# Librerias
import MySQLdb
class BD():

    def insert_data(self, id):
        db = MySQLdb.connect( "localhost", "root", "1234", "RFID" )
        query = "INSERT INTO Acceso (idUsuario, fechatiempo) VALUES ('%s', SYSDATE(), '%s')" % (id)
        cursor = db.cursor()
        try:
            cursor.execute(query)
            db.commit()
        except:
            db.rollback()
        db.close()

    def select_data(self):
        db = MySQLdb.connect( "localhost", "root", "1234", "RFID" )
        query = "SELECT * FROM Acceso"
        cursor = db.cursor()
        try:
            cursor.execute(query)
            results = cursor.fetchall()
        except:
            print "No se pudo obtener la informacion de la base de datos."
        return results
