#!/bin/sh
NAME="some-redis"
PORT="-p 6379:6379"
VOLUME="-v `pwd`/db:/data"
IMAGE="redis:latest"

docker run -d --name $NAME $PORT $VOLUME $IMAGE
