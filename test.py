import MySQLdb

def select_prueba():
    db = MySQLdb.connect( "MYSQL7003.site4now.net", "a3b3f6_h1326", "h1326223", "db_a3b3f6_h1326" )
    query = "SELECT * FROM Usuario"
    cursor = db.cursor()
    try:
        cursor.execute(query)
        results = cursor.fetchall()
    except:
        print "No se pudo obtener la informacion de la base de datos."
    return results

response = select_prueba()
print response
