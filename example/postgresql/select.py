import psycopg2
from pprint import pprint

conn = psycopg2.connect(host="localhost", database="vault", user="postgres", password="postgres")
cursor = conn.cursor()
cursor.execute("select * from things")

pprint(cursor.fetchall())

cursor.close()
conn.close()
