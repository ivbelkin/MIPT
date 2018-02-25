#! /bin/bash

docker build -t ivbelkin/todolist .

docker run --rm -d -v $(pwd)/app:/app -p 8000:8000 --name ivbelkin_todolist ivbelkin/todolist python website/manage.py runserver 0:8000
docker exec ivbelkin_todolist python website/manage.py makemigrations todolist
docker exec ivbelkin_todolist python website/manage.py migrate
docker attach ivbelkin_todolist
