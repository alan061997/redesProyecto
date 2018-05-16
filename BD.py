#!/usr/bin/python
# Librerias
import MySQLdb
class BD():
    def get_estatus(id):
        db = MySQLdb.connect( "MYSQL7003.site4now.net", "a3b3f6_h1326", "h1326223", "db_a3b3f6_h1326" )
        query = "SELECT COUNT(*) FROM Acceso"
        cursor = db.cursor()
        try:
            cursor.execute(query)
            results = cursor.fetchall()
            if results[0][0] == 0:
                return "ENTRADA"
            else:
                query = "SELECT estatus FROM Acceso WHERE idUsuario = '%s' ORDER BY ID DESC LIMIT 1" % (id)
                cursor = db.cursor()
                cursor.execute(query)
                results = cursor.fetchall()
                print results
                if results[0][0] == "ENTRADA":
                    return "SALIDA"
                elif results[0][0] == "SALIDA":
                    return "ENTRADA"
        except:
            print "No se pudo obtener la informacion de la base de datos."
            return None
        db.close()

    def insert_data(self, id):
        if id == '':
            print "ID: NULL"
            return

        db = MySQLdb.connect( "MYSQL7003.site4now.net", "a3b3f6_h1326", "h1326223", "db_a3b3f6_h1326" )
        query = "INSERT INTO Acceso (idUsuario, fechatiempo, estatus) VALUES ('%s', SYSDATE(), '%s')" % (id, self.get_estatus())
        cursor = db.cursor()
        try:
            cursor.execute(query)
            db.commit()
        except:
            db.rollback()
        db.close()
