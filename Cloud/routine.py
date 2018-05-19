import MySQLdb

db = MySQLdb.connect(host="localhost",
                     user="root",
                     passwd="1234",
                     db="RFID")

cur = db.cursor()

# Rutin 1: Sube todos los registros actuales a la nube.
cur.execute("SELECT * FROM Acceso")
for row in cur.fetchall():
    print row[0]

db.close()