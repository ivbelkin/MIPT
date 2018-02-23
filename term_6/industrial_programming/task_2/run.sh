#! /bin/bash

docker build -t ivbelkin/todolist .

docker run -it --rm -v $(pwd)/app:/app -p 8000:8000 ivbelkin/todolist
