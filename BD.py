#!/usr/bin/python
# Librerias
import MySQLdb
class BD():

    def insert_data(self, id):
        db = MySQLdb.connect( "MYSQL7003.site4now.net", "a3b3f6_h1326", "h1326223", "db_a3b3f6_h1326" )
        query = "INSERT INTO Acceso (idUsuario, fechatiempo) VALUES ('%s', SYSDATE(), '%s')" % (id)
        cursor = db.cursor()
        try:
            cursor.execute(query)
            db.commit()
        except:
            db.rollback()
        db.close()

    def select_data(self):
        db = MySQLdb.connect( "MYSQL7003.site4now.net", "a3b3f6_h1326", "h1326223", "db_a3b3f6_h1326" )
        query = "SELECT * FROM Acceso"
        cursor = db.cursor()
        try:
            cursor.execute(query)
            results = cursor.fetchall()
        except:
            print "No se pudo obtener la informacion de la base de datos."
        return results
