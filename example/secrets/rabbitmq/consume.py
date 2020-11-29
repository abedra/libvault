import pika
import hvac
import os
from dotenv import load_dotenv
from pathlib import Path

load_dotenv(dotenv_path=Path('../..') / '..' / '.env')

role_id = os.getenv("APPROLE_ROLE_ID")
secret_id = os.getenv("APPROLE_SECRET_ID")

vault_client = hvac.Client(url="http://localhost:8200")
vault_client.auth_approle(role_id=role_id, secret_id=secret_id)

rabbit_credentials = vault_client.read(path="rabbitmq/creds/my-role")
rabbit_username = rabbit_credentials["data"]["username"]
rabbit_password = rabbit_credentials["data"]["password"]

credentials = pika.PlainCredentials(username=rabbit_username, password=rabbit_password)
connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost', credentials=credentials))
channel = connection.channel()
channel.queue_declare(queue='hello')


def callback(ch, method, properties, body):
    print(" [x] Received %r" % body)


channel.basic_consume(queue='hello', on_message_callback=callback, auto_ack=True)

print(' [*] Waiting for messages. To exit press CTRL+C')
channel.start_consuming()
