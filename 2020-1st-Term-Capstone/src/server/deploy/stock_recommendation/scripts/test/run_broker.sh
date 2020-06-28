#!/bin/sh
NAME="rabbitserver"
IMAGE="rabbitmq"
PORT="-p 5672:5672 -p 15672:15672"
USER="user"
PASSWORD="password"
docker run -d $PORT --name $NAME -e RABBITMQ_DEFAULT_USER=$USER -e RABBITMQ_DEFAULT_PASS=$PASSWORD $IMAGE
