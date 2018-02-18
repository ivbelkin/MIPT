# functions shared beetween sender and receiver
import pika
from sys import stdin, stdout, stderr
from time import sleep


def connect(host):
    # wait for launching rabbit and knock niter times
    niter = 10
    for i in range(niter):
        try:
            stdout.write("Connecting to RabbitMQ...")
            connection = pika.BlockingConnection(
                pika.ConnectionParameters(host=host))
            stdout.write("Established.\n")
            return connection
        except pika.exceptions.AMQPError:
            stdout.write("Refused.\n")
            sleep(5)
    stdout.write("Unable to connect to RabbinMQ. Halt.\n")
    exit(1)
