import pika
from sys import stdin, stdout, stderr
from utils import connect

def main():
    connection = connect("localhost")

    stdout.write("Connecting to queue...")
    channel = connection.channel()
    channel.queue_declare(queue="hello")
    stdout.write("Established.\n")

    msg = None
    stdout.write("\nEnter a message. To exit press CTRL+C.\n")
    while True:
        try:
            if not msg:
                msg = input(">>> ")
            channel.basic_publish(exchange="",
                routing_key="hello",
                body=msg.encode())
            msg = None
        except KeyboardInterrupt:
            stdout.write("\n")
            connection.close()
            break
        except pika.exceptions.AMQPError:
            connection = connect("localhost")
            channel = connection.channel()
            continue


if __name__ == "__main__":
    main()
