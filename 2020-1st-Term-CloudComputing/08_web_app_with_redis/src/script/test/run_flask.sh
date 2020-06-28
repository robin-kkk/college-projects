#!/bin/sh
NAME="app"
IMAGE="ke2ek/flaskapp:1.0"
PORT="-p 80:80"
VOLUME="-v `pwd`/pyflask:/code"
OPTION="-idt"
CMD="python app.py"

docker run $OPTION $PORT $VOLUME --name $NAME $IMAGE $CMD
