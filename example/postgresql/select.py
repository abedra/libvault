import hvac
import os
import psycopg2

from dotenv import load_dotenv
from pathlib import Path
from pprint import pprint

load_dotenv(dotenv_path=Path('..') / '..' / '.env')

role_id = os.getenv("APPROLE_ROLE_ID")
secret_id = os.getenv("APPROLE_SECRET_ID")

vault_client = hvac.Client(url="http://localhost:8200")
vault_client.auth_approle(role_id=role_id, secret_id=secret_id)

postgres_credentials = vault_client.read(path="database/creds/readonly")
postgres_username = postgres_credentials["data"]["username"]
postgres_password = postgres_credentials["data"]["password"]

conn = psycopg2.connect(host="localhost", database="vault", user=postgres_username, password=postgres_password)
cursor = conn.cursor()
cursor.execute("select * from things")

pprint(cursor.fetchall())

cursor.close()
conn.close()
