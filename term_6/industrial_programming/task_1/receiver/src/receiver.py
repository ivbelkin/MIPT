import pika
from sys import stdout, stderr, stdin
from utils import connect
import psycopg2


postgres_connection = None
rabbit_connection = None
cur = None


def callback(ch, method, properties, body):
    stdout.write("Message: {}\n".format(body.decode()))
    cur.execute("INSERT INTO ivbelkin_db (data) VALUES (%s)", (body.decode(),))
    postgres_connection.commit()


def main():
    global postgres_connection
    global rabbit_connection
    global cur
    
    rabbit_connection = connect("rabbit")

    stdout.write("Connecting to queue...")
    channel = rabbit_connection.channel()
    channel.queue_declare(queue='hello')
    channel.basic_consume(callback, queue='hello', no_ack=True)
    stdout.write("Established.\n")

    stdout.write("Connecting to database...")
    postgres_connection = psycopg2.connect(host="db", user="postgres", 
        password="postgres", dbname="ivbelkin_db")
    cur = postgres_connection.cursor()    
    stdout.write("Established.\n")

    stdout.write("Creating table 'ivbelkin_db'...")    
    cur.execute("CREATE TABLE IF NOT EXISTS ivbelkin_db (data varchar);")
    postgres_connection.commit()
    stdout.write("Completed.\n")
    
    stdout.write("Waiting for messages.\n")
    try:
        channel.start_consuming()
    except KeyboardInterrupt:
        cur.close()
        postgres_connection.close()


if __name__ == "__main__":
    main()
