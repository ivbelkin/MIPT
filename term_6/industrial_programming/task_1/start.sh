#! /bin/bash

# build an image of sender -- client app
docker build -t ivbelkin/sender ./sender

# up rabbit, receiver and postgres -- server app
docker-compose up --build -d

# run client and attach it to server via localhost
docker run -it -v "`pwd`/sender/src:/app/src" --net=host --rm ivbelkin/sender
